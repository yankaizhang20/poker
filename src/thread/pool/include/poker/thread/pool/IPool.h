//
// Created by zyk on 2024-6-6
//

#pragma once

#include "./Task.h"


namespace poker::thread::pool
{
    class Worker;

    class IPool
    {
    public:
        /**
         * @brief 通知线程池给定的 worker 对指定任务执行结束
         */
        virtual void NotifyTaskDone(Worker *worker, std::shared_ptr< ITask > task) = 0;

        /**
         * @return 线程池统一的异常处理器
         */
        virtual std::function< void(const std::exception &) > GetExceptionHandler() = 0;
    };
}   // namespace poker::thread::pool
