//
// Created by zyk on 24-6-6.
//

#pragma once

#include <moodycamel/concurrentqueue.h>

#include <poker/thread/condition.h>

#include "./pool/IPool.h"
#include "./pool/Task.h"
#include "./pool/Worker.h"


namespace poker::thread
{
    class Pool : private pool::IPool
    {
    public:
        explicit Pool(std::size_t workers_count = 0);

        ~Pool();

        void SetExceptionHandler(std::function< void(const std::exception &ec) > handler);

    public:
        /**
         * @brief 调整线程池的大小；若线程池缩小时，需要等待有足够的线程
         * 退出时，本函数才能返回。
         */
        void Resize(std::size_t workers_count);

        /**
         * @brief 等待任务执行，直至数量少于或等于给定值时返回
         * @param remaining_tasks_count 期望的剩余任务数量
         * @return 是否等待成功（线程池关闭前，将强制退出它的所有等待流程）
         */
        bool Wait(std::size_t remaining_tasks_count = 0) const;

        /**
         * @return 线程池持有的任务数量（包括待执行与正在执行的）
         */
        std::size_t RemainingTasksCount() const;

        /**
         * @return 线程池剩余可用的线程数量
         */
        std::size_t RemainingWorkersCount() const;

        /**
         * @return 线程池大小
         */
        std::size_t Size() const;

    public:
        /**
         * @brief 添加一个新的待执行任务
         */
        void Add(std::shared_ptr< ITask > task);

        /**
         * @brief 添加一个空返回值的待执行任务
         */
        template < class TFunction,
                   poker_requires(std::is_same_v< typename std::invoke_result< TFunction >::type, void >) >
        std::shared_ptr< BasicTask > Add(TFunction                                       func,
                                         std::function< void(const std::exception &ec) > func_exception = nullptr)
        {
            auto task = std::make_shared< BasicTask >(std::move(func), std::move(func_exception));
            Add(task);
            return task;
        }

        /**
         * @brief 添加一个带返回值的待执行任务
         */
        template < class TFunction,
                   poker_requires(not std::is_same_v< typename std::invoke_result< TFunction >::type, void >) >
        std::shared_ptr< FutureTask< typename std::invoke_result< TFunction >::type > >
        Add(TFunction func, std::function< void(const std::exception &ec) > func_exception = nullptr)
        {
            using FutureTaskType = FutureTask< typename std::invoke_result< TFunction >::type >;
            auto task            = std::make_shared< FutureTaskType >(std::move(func), std::move(func_exception));
            Add(task);
            return task;
        }

        /**
         * @brief 取消所有任务
         */
        void CancelAll();

    private:
        /**
         * @brief 异步的处理主流程，负责运行任务
         */
        void AsyncMain();

        /**
         * @brief 执行一个任务
         */
        void ExecuteOneTask();

    private:
        /**
         * @brief 检查线程池是否健在，若已经被析构，或正在析构，将抛出错误
         */
        void CheckPoolAlive();

        /**
         * @brief 追加任务执行器到可用列表中，并唤醒等待线程
         */
        void PushWorker(pool::Worker *worker);

        /**
         * @brief 取出一个任务执行器，若当前无可用执行器，将阻塞等待
         * @return 若返回空值，说明主流程要求退出，且仍然无作业器可用
         */
        pool::Worker *PopWorker();

        /**
         * @brief 处理任务执行完毕的情况，将 worker 添加回可用队列中
         */
        void NotifyTaskDone(pool::Worker *worker, std::shared_ptr< ITask > task) override;

        /**
         * @return 统一设置的异常处理器
         */
        std::function< void(const std::exception &) > GetExceptionHandler() override;

    private:
        // 任务执行器总数，被互斥量保护
        std::size_t _total_workers_count = 0;

        // 保护 Resize() 流程唯一性的互斥量
        mutable std::mutex _mutex_pool_size;

    private:
        // 可用的任务执行器
        moodycamel::ConcurrentQueue< pool::Worker * > _available_workers;

        // 任务执行器的可用信用量
        Semaphore _have_workers_available;

        // 任务执行器的剩余信用量，与可用信用量拥有相同的值，
        // 但该信号量专门用于析构时，等待所有 workers 回归
        Semaphore _have_workers_remain;

    private:
        // 待执行的任务
        moodycamel::ConcurrentQueue< std::shared_ptr< ITask > > _tasks;

        // 待执行的任务信号量
        Semaphore _have_tasks_queuing;

        // 线程池持有的所有任务数量（包括正在排队、以及正在执行的）
        Counter _tasks_count = 0;

    private:
        // 线程池销毁标志
        std::atomic_bool _pool_requires_quiting = false;

        // 线程池主流程
        std::thread _main_thread;

        // 异常处理器
        std::function< void(const std::exception &) > _exception_handler;
        SharedMutexLock                               _exception_handler_keeper;
    };
}   // namespace poker::thread