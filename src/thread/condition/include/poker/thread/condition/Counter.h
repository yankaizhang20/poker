//
// Created by zyk on 24-6-6.
//

#pragma once

#include <condition_variable>
#include <functional>


namespace poker::thread
{
    class Counter
    {
    public:
        Counter(std::size_t initial_count = 0);

        ~Counter();

    public:
        void Increase(std::size_t n);

        void Decrease(std::size_t n);

        void IncreaseOne();

        void DecreaseOne();

        void Reset(std::size_t initial_count);

        void FailAll();

    public:
        bool WaitUntilLessThan(std::size_t n) const;

        bool WaitUntilLessEqualThan(std::size_t n) const;

        bool WaitUntilEqual(std::size_t n) const;

        bool WaitUntilLargerThan(std::size_t n) const;

        bool WaitUntilLargerEqualThan(std::size_t n) const;

    public:
        operator std::size_t () const;

    private:
        bool InnerWait(const std::function< bool() > &condition) const;

    private:
        // 用于处理线程等待
        mutable std::mutex              _mutex;
        mutable std::condition_variable _con;

        // 计数器
        std::size_t _count = 0;

        // 用于退出信号量的等待
        bool _failed = false;
    };
}   // namespace poker::thread