//
// Created by zyk on 24-6-6.
//

#include <poker/macro.h>

#include "poker/thread/Pool.h"


namespace poker::thread
{
    Pool::Pool(std::size_t workers_count)
    {
        // 不限制信号量的上限，从零开始处理信号
        _have_workers_available.Reset(-1);
        _have_workers_remain.Reset(-1);
        _have_tasks_queuing.Reset(-1);

        _main_thread = std::thread([ & ]() { AsyncMain(); });

        Resize(workers_count);
    }

    Pool::~Pool()
    {
        // 即将清除所有的 workers，回收全部资源，因此也是一种 resize，
        // 我们同样使用 resize 互斥量
        std::unique_lock lock(_mutex_pool_size);

        // 设置线程池销毁标志
        _pool_requires_quiting = true;

        // 唤醒主线程，将其从对新任务的等待中退出
        _have_tasks_queuing.FailAll();
        _have_workers_available.FailAll();

        // 解除所有对任务执行的等待流程
        _tasks_count.FailAll();

        // 等待主线程完全退出
        if (_main_thread.joinable())
            _main_thread.join();

        // 销毁所有 worker
        pool::Worker *worker = nullptr;

        while (_total_workers_count != 0)
        {
            bool acquire_worker_should_be_successful = _have_workers_remain.Acquire();
            assert(acquire_worker_should_be_successful);

            bool get_worker_should_be_successful = _available_workers.try_dequeue(worker);
            assert(get_worker_should_be_successful);
            assert(worker != nullptr);

            delete worker;
            --_total_workers_count;
        }
    }

    void Pool::SetExceptionHandler(std::function< void(const std::exception &) > handler)
    {
        WriterLocking(_exception_handler_keeper)
        {
            _exception_handler = std::move(handler);
        }
    }

    void Pool::Resize(std::size_t workers_count)
    {
        // 若池即将析构时 Resize() 将报错
        CheckPoolAlive();

        // 我们需要确保本流程的唯一性，从使用角度来讲，应该不会出现
        // 频繁的 Resize() 情况，因此我们可以使用互斥量加锁。
        std::unique_lock lock(_mutex_pool_size);

        // 期望大小与现在池大小相同时，则没有任何操作
        if (_total_workers_count == workers_count)
            return;

        // 期望大小比现在的池大小更大时，则需要创建更多 workers，
        // 该过程无需等待
        if (_total_workers_count < workers_count)
        {
            // 当 workers 数量少于期待的数量时，创建更多的 workers
            for (std::size_t n = 0, n_end = workers_count - _total_workers_count; n < n_end; ++n)
            {
                PushWorker(new pool::Worker(this));
            }

            // 更新 workers 总数量信息，并唤醒主线程
            _total_workers_count = workers_count;

            return;
        }

        // 若期望大小比现在的池大小更小时，则需要尝试裁剪 workers，
        // 首先从已有的可用 workers 裁剪；若可用 workers 不够裁剪时，需要
        // 进行阻塞等待。
        do
        {
            // 从逻辑上，先去除一个 worker
            --_total_workers_count;

            // 尝试从 worker 队列中弹出一个 worker，若弹出失败，
            // 将阻塞至 worker 被释放，并重新添加到队列中
            pool::Worker *removed_worker_ptr = PopWorker();

            // 删除刚从队列中弹出的 worker
            delete removed_worker_ptr;
        } while (_total_workers_count > workers_count);
    }

    bool Pool::Wait(std::size_t remaining_tasks_count) const
    {
        return _tasks_count.WaitUntilLessEqualThan(remaining_tasks_count);
    }

    std::size_t Pool::RemainingTasksCount() const
    {
        return _tasks_count;
    }

    std::size_t Pool::RemainingWorkersCount() const
    {
        std::unique_lock lock(_mutex_pool_size);

        if (std::size_t tasks_count = _tasks_count; tasks_count >= _total_workers_count)
            return 0;
        else
            return _total_workers_count - tasks_count;
    }

    std::size_t Pool::Size() const
    {
        std::unique_lock lock(_mutex_pool_size);
        return _total_workers_count;
    }

    void Pool::Add(std::shared_ptr< ITask > task)
    {
        _tasks_count.IncreaseOne();
        _tasks.enqueue(std::move(task));
        _have_tasks_queuing.Release();
    }

    void Pool::CancelAll()
    {
        std::shared_ptr< ITask > next_task;

        if (_tasks.try_dequeue(next_task))
        {
            next_task->Cancel();
            _tasks_count.DecreaseOne();
        }
    }

    void Pool::AsyncMain()
    {
        while (not _pool_requires_quiting and _have_tasks_queuing.Acquire())
        {
            ExecuteOneTask();
        }
    }

    void Pool::ExecuteOneTask()
    {
        // 取出新任务
        std::shared_ptr< ITask > next_task;

        // 若取出失败，可能是被删除了
        if (not _tasks.try_dequeue(next_task))
            return;

        // 取出一个可用 worker；
        // 若没有 worker 可用，将阻塞直到有可用的 worker 产生为止
        pool::Worker *worker = PopWorker();

        // 若获得的 worker 为空，说明当前要求线程池退出，需立即结束流程
        if (worker == nullptr)
            return;

        // 异步执行该任务
        worker->AsyncRun(std::move(next_task));
    }

    void Pool::CheckPoolAlive()
    {
        if (_pool_requires_quiting)
            poker_never("use pool while deleting it !");
    }

    void Pool::PushWorker(pool::Worker *worker)
    {
        _available_workers.enqueue(worker);
        _have_workers_remain.Release();
        _have_workers_available.Release();
    }

    pool::Worker *Pool::PopWorker()
    {
        // 等待可用任务执行器
        if (pool::Worker *worker = nullptr; _have_workers_available.Acquire())
        {
            bool acquire_worker_should_be_successful = _have_workers_remain.TryAcquire();
            assert(acquire_worker_should_be_successful);

            bool get_worker_should_be_successful = _available_workers.try_dequeue(worker);
            assert(get_worker_should_be_successful);
            assert(worker != nullptr);

            return worker;
        }
        else
            return nullptr;
    }

    void Pool::NotifyTaskDone(pool::Worker *worker, std::shared_ptr< ITask > task)
    {
        _tasks_count.DecreaseOne();
        PushWorker(worker);
    }

    std::function< void(const std::exception &) > Pool::GetExceptionHandler()
    {
        ReaderLocking(_exception_handler_keeper)
        {
            return _exception_handler;
        }
    }
}   // namespace poker::thread