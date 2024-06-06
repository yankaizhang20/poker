//
// Created by zyk on 2024-6-6
//

#pragma once

#include <functional>
#include <future>
#include <set>

#include <poker/thread/condition.h>
#include <poker/thread/lock.h>


namespace poker::thread
{
    enum class TaskState
    {
        Queuing,
        Running,
        Canceled,
        Broken,
        Finished,
    };

    class ITask;

    namespace details
    {
        /**
         * @brief 执行指定任务，若任务执行过程中抛出异常，将尝试由任务的
         * 异常处理器处理，若任务的异常处理器无法处理，将继续抛出。
         * @return
         *   @retval TaskState::Canceled: 任务已经取消
         *   @retval TaskState::Broken:   任务因异常中断，但被任务异常处理器正确处理
         *   @retval TaskState::Finished: 任务正确执行完毕
         */
        TaskState ExecuteTask(ITask &task);
    }   // namespace details

    class ITask
    {
    public:
        /**
         * @brief 取消本任务，返回是否取消成功。
         * @return
         *   @retval true:  任务成功取消（已经取消的任务再次取消，仍然返回成功）
         *   @retval false: 任务已经在执行状态，或执行完毕（异常结束也算结束），则取消失败
         */
        bool Cancel();

        /**
         * @brief 等待任务执行完毕、被中断、或出现异常而终止。
         * @return
         *   @retval true:  任务正常执行完毕
         *   @retval false: 任务被取消、或者出现异常而终止
         */
        bool Wait() const;

        /**
         * @brief 在一定的时间限制内，等待任务执行完毕、被中断、或出现异常而终止。
         * @param timeout 等待超时时间
         * @return
         *   @retval true:  在给定的超时时间内，任务正常执行完毕
         *   @retval false: 等待超时、或任务被取消、或出现异常终止
         */
        bool Wait(const unit::Time &timeout) const;

        /**
         * @return 获取任务状态
         */
        TaskState GetState() const;

    protected:
        /**
         * @brief 设置主流程函数（非线程安全）
         */
        void SetMainFunction(std::function< void() > func);

        /**
         * @brief 设置异常处理器（非线程安全）
         */
        void SetExceptionHandler(std::function< void(const std::exception &) > func);

    private:
        /**
         * @brief 由一个友元的细节域下的函数负责任务的具体运行
         */
        friend TaskState details::ExecuteTask(ITask &task);

    private:
        // 执行与取消锁，用于执行或取消流程的判定
        thread::SharedMutexLock _executing_and_canceling_mutex;

        // 任务的状态
        TaskState _state = TaskState::Queuing;

        // 任务的执行流程
        std::function< void() > _func;

        // 任务的异常处理流程
        std::function< void(const std::exception &) > _func_exception;

        // 用于等待流程的条件变量
        Condition _is_finished;
    };

    /**
     * @brief 基础的可执行任务，不带任何返回值的处理。
     */
    class BasicTask : public ITask
    {
    public:
        explicit BasicTask(std::function< void() >                       func,
                           std::function< void(const std::exception &) > func_exception = nullptr);
    };

    /**
     * @brief 拥有指定类型返回值的任务，可通过 future api 获取执行的结果
     */
    template < class T >
    class FutureTask : public ITask
    {
    public:
        explicit FutureTask(std::function< T() >                            func,
                            std::function< void(const std::exception &ec) > exception_handler = nullptr)
        {
            SetMainFunction(std::move(func));
            ITask::SetExceptionHandler(std::move(exception_handler));
        }

    public:
        /**
         * @brief 等待任务执行完毕，并返回结果
         * @return 可能的结果
         */
        std::optional< T > WaitResult()
        try
        {
            return _result_future.get();
        }
        catch (...)
        {
            return {};
        }

        /**
         * @brief 在一定时间限制内，等待任务执行完毕，并返回结果
         * @param timeout 等待的时间范围
         * @return 可能的结果；若等待超时，也返回错误
         */
        std::optional< T > WaitResult(const unit::Time &timeout)
        try
        {
            std::future_status status = _result_future.wait_until(utils::TimePointFromNow(timeout));

            if (status == std::future_status::ready)
                return _result_future.get();
            else
                return {};
        }
        catch (...)
        {
            return {};
        }

    private:
        void SetMainFunction(std::function< T() > func)
        {
            if (func == nullptr)
                return;

            // 设置结果获取器
            _result_future = _result_promise.get_future();

            ITask::SetMainFunction(
                    [ &, func = std::move(func) ]()
                    {
                        try
                        {
                            _result_promise.set_value(func());
                        }
                        catch (...)
                        {
                            _result_promise.set_exception({});
                            throw;
                        }
                    });
        }

    private:
        std::promise< T > _result_promise;
        std::future< T >  _result_future;
    };

    class TaskGroup
    {
    public:
        TaskGroup &operator+= (std::shared_ptr< ITask > task_ptr);

        void Wait();

    private:
        std::set< std::shared_ptr< ITask > > _tasks;
    };
}   // namespace poker::thread
