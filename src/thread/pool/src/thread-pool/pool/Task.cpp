//
// Created by zyk on 24-6-6.
//

#include "poker/thread/pool/Task.h"


namespace poker::thread
{
    bool ITask::Cancel()
    {
        WriterLocking(_executing_and_canceling_mutex)
        {
            switch (_state)
            {
                case TaskState::Queuing:
                case TaskState::Canceled:
                    _state = TaskState::Canceled;
                    _is_finished.Fail();
                    return true;

                case TaskState::Running:
                case TaskState::Broken:
                case TaskState::Finished:
                    return false;

                default:
                    poker_no_impl();
            }
        }
    }

    bool ITask::Wait() const
    {
        return _is_finished.Wait();
    }

    bool ITask::Wait(const unit::Time &timeout) const
    {
        return _is_finished.Wait(timeout);
    }

    TaskState ITask::GetState() const
    {
        ReaderLocking(_executing_and_canceling_mutex)
        {
            return _state;
        }
    }

    void ITask::SetMainFunction(std::function< void() > func)
    {
        _func = std::move(func);
    }

    void ITask::SetExceptionHandler(std::function< void(const std::exception &) > func)
    {
        _func_exception = std::move(func);
    }
}   // namespace poker::thread


namespace poker::thread::details
{
    TaskState ExecuteTask(ITask &task)
    {
        // 执行任务主要流程，并正确处理可能出现的异常
        static auto ExecuteTaskWithoutTryCatch = [](ITask &task)
        {
            if (task._func)
                task._func();

            task._state = TaskState::Finished;
            task._is_finished.Satisfy();
        };

        WriterLocking(task._executing_and_canceling_mutex)
        {
            // 只允许执行正在排队的任务；若任务要求取消，则直接结束流程
            switch (task._state)
            {
                case TaskState::Queuing:
                    task._state = TaskState::Running;
                    break;

                case TaskState::Running:
                    poker_never("execute a task twice");

                case TaskState::Canceled:
                    return TaskState::Canceled;

                case TaskState::Broken:
                    poker_never("execute a broken task");

                case TaskState::Finished:
                    poker_never("execute a finished task");

                default:
                    poker_no_impl();
            }

            if (task._func_exception)
            {
                try
                {
                    ExecuteTaskWithoutTryCatch(task);
                }
                catch (const std::exception &ec)
                {
                    task._state = TaskState::Broken;
                    task._is_finished.Fail();
                    task._func_exception(ec);
                }
            }
            else
            {
                ExecuteTaskWithoutTryCatch(task);
            }

            return task._state;
        }
    }
}   // namespace poker::thread::details

namespace poker::thread
{
    BasicTask::BasicTask(std::function< void() > func, std::function< void(const std::exception &) > func_exception)
    {
        SetMainFunction(std::move(func));
        SetExceptionHandler(std::move(func_exception));
    }

    TaskGroup &TaskGroup::operator+= (std::shared_ptr< ITask > task_ptr)
    {
        if (task_ptr != nullptr)
            _tasks.insert(std::move(task_ptr));

        return *this;
    }

    void TaskGroup::Wait()
    {
        for (const std::shared_ptr< ITask > &ptr : _tasks)
            ptr->Wait();
    }
}   // namespace poker::thread