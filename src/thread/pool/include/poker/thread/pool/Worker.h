//
// Created by zyk on 2024-6-6
//

#pragma once

#include <poker/thread/condition.h>

#include "./IPool.h"
#include "./Task.h"


namespace poker::thread::pool
{
    class Worker
    {
    public:
        explicit Worker(IPool *pool_interface);

        ~Worker();

    public:
        /**
         * @brief 异步执行给定任务
         */
        void AsyncRun(std::shared_ptr< ITask > task);

    private:
        /**
         * @brief 异步的任务处理主流程
         */
        void AsyncMain();

        /**
         * @brief 处理一次任务的执行
         */
        void MainRunOnce();

        /**
         * @brief 通知线程池本 worker 的任务执行完毕
         */
        void NotifyPoolTaskDone();

    private:
        void ExecuteTaskWithoutTryCatch();

    private:
        // 线程池操作接口
        IPool *_pool_interface = nullptr;

        // 本任务执行器的异步执行线程
        std::thread _working_thread;

        // 任务执行等待条件
        Condition _has_task_to_execute;

        // 当前正在处理的任务
        std::shared_ptr< ITask > _current_task;
    };
}   // namespace poker::thread::pool
