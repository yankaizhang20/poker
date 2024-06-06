//
// Created by zyk on 24-6-6.
//

#pragma once

#include <condition_variable>

#include <poker/unit.h>


namespace poker::thread
{
    class Semaphore
    {
    public:
        explicit Semaphore(long max_count = 1);

        /**
         * @brief 设置信号量的最大值，若最大值为负数，则认为无上界
         */
        void SetMaxCount(long max_count);

        /**
         * @brief 获取一次信号，若当前信号量不足时，将进行等待
         * @return 是否获取成功（可能因为要求解除信号量等待而返回失败）
         */
        bool Acquire();

        /**
         * @brief 在有限时间内进行一次 Acquire()
         * @return 是否获取成功
         */
        bool Acquire(const unit::Time &time);

        /**
         * @brief 尝试获取一次信号，若获取失败，不会阻塞等待，会立即返回
         * @return 是否获取成功
         */
        bool TryAcquire();

        /**
         * @brief 释放一次信号
         */
        void Release();

        /**
         * @brief 解除所有其他对信号量的等待流程
         */
        void FailAll();

        /**
         * @brief 重置信号量状态
         */
        void Reset();

        /**
         * @brief 重置信号量状态，同时设置新的上限值
         */
        void Reset(long max_count);

        ~Semaphore();

    private:
        void InnerReset(long max_count);

    private:
        // 用于处理线程等待
        std::mutex              _mutex;
        std::condition_variable _con;

        // 信号量当前值与上限值
        long _max_count = 1;
        long _count     = 0;

        // 用于退出信号量的等待
        bool _failed = false;
    };
}   // namespace poker::thread