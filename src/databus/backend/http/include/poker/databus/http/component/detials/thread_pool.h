//
// Created by zyk on 24-3-13.
//

#pragma once

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>

#include "./httplib_safe.h"


static constexpr std::size_t sys_max_thread_nums = 32;

namespace poker::databus::http::details
{
    class HttpRequestProcess
    {
    public:
        virtual ~HttpRequestProcess() = default;

        virtual void AddRequest(std::packaged_task< ::httplib::Result() > &&task) = 0;
    };

    class ThreadPool : public HttpRequestProcess
    {
    public:
        ~ThreadPool() override
        {
            std::lock_guard tasks_keeper(_tasks_keeper);
            if (!_shutdown)
            {
                ShutDown();
            }
        }

        void Init(std::size_t thread_nums)
        {
            _cur_thread_nums = std::min(sys_max_thread_nums, thread_nums);

            // 创建线程
            for (std::size_t i = 0; i < _cur_thread_nums; i++)
            {
                _threads.emplace_back(Worker(this));
            }
        }

        void AddRequest(std::packaged_task< ::httplib::Result() > &&task) override
        {
            // 添加任务
            {
                std::lock_guard tasks_keeper(_tasks_keeper);
                _tasks.push(std::move(task));
            }

            // 通知线程处理
            _task_existed.notify_one();
        }

        void ShutDown()
        {
            {
                std::lock_guard shutdown_keeper(_tasks_keeper);

                _shutdown = true;
            }

            _task_existed.notify_all();

            for (std::size_t i = 0; i < _cur_thread_nums; i++)
            {
                _threads[ i ].join();
            }
        }

    private:
        class Worker
        {
        public:
            explicit Worker(ThreadPool *const p_core) : _p_core(p_core)
            {
            }

            void operator() ()
            {
                std::packaged_task< ::httplib::Result() > task;

                while (true)
                {
                    std::unique_lock task_keeper(_p_core->_tasks_keeper);

                    _p_core->_task_existed.wait(task_keeper,
                                                [ this ]() { return !_p_core->_tasks.empty() or _p_core->_shutdown; });

                    // 若请求结束线程池，并且没有任务需要处理了，则退出线程
                    if (_p_core->_shutdown and _p_core->_tasks.empty())
                    {
                        break;
                    }

                    // 取任务
                    task = std::move(_p_core->_tasks.front());
                    _p_core->_tasks.pop();

                    task_keeper.unlock();

                    // 执行任务
                    task();
                }
            }

        private:
            ThreadPool *const _p_core;
        };

    private:
        std::condition_variable _task_existed;

        std::mutex _tasks_keeper;

        // 标识是否被请求结束
        bool _shutdown = false;

        // 任务队列
        std::queue< std::packaged_task< ::httplib::Result() > > _tasks;

        // 工人数量
        std::size_t _cur_thread_nums = 0;

        // 打工人
        std::vector< std::thread > _threads;
    };
}   // namespace poker::databus::http::details