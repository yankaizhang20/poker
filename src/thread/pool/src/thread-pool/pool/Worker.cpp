//
// Created by zyk on 24-6-6.
//

#include "poker/thread/pool/Worker.h"


namespace poker::thread::pool
{
    Worker::Worker(IPool *pool_interface) : _pool_interface(pool_interface)
    {
        assert(_pool_interface != nullptr);

        // 启动异步的任务处理线程
        _working_thread = std::thread([ & ]() { AsyncMain(); });
    }

    Worker::~Worker()
    {
        _has_task_to_execute.Fail();

        if (_working_thread.joinable())
            _working_thread.join();
    }

    void Worker::AsyncRun(std::shared_ptr< ITask > task)
    {
        _current_task = std::move(task);
        _has_task_to_execute.Satisfy();
    }

    void Worker::AsyncMain()
    {
        while (_has_task_to_execute.Wait())
        {
            _has_task_to_execute.Init();
            MainRunOnce();
        }
    }

    void Worker::MainRunOnce()
    {
        std::function< void(const std::exception &ec) > handler = _pool_interface->GetExceptionHandler();

        if (handler == nullptr)
            ExecuteTaskWithoutTryCatch();
        else
        {
            try
            {
                ExecuteTaskWithoutTryCatch();
            }
            catch (const std::exception &ec)
            {
                handler(ec);
            }
        }

        NotifyPoolTaskDone();
    }

    void Worker::NotifyPoolTaskDone()
    {
        _pool_interface->NotifyTaskDone(this, std::move(_current_task));
    }

    void Worker::ExecuteTaskWithoutTryCatch()
    {
        if (_current_task != nullptr)
            details::ExecuteTask(*_current_task);
    }
}   // namespace poker::thread::pool