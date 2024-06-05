//
// Created by zyk on 24-6-5.
//

#pragma once

#include <condition_variable>

#include <poker/unit.h>


namespace poker::thread
{
    class Condition
    {
    public:
        Condition(bool value = false);   // NOLINT(google-explicit-constructor)

        /**
         * @brief 析构时，将调用 Fail() 函数，使其他正在等待条件变换的线程退出等待。
         */
        ~Condition();

        /**
         * @brief 设置当前条件的值；但是，即使设置为 true，也不会释放其它正在等待条件满足的线程。
         * @param value
         */
        void Init(bool value = false);

        /**
         * @brief 等待条件成立。
         * @return 条件是否被满足；返回 false 只有一种可能：条件被标识为永远无法被满足（也即调用了 Fail() 函数）。
         */
        bool Wait() const;

        /**
         * @brief 在指定时间内，等待条件成立。
         * @param time
         * @return 返回超时之后，条件是否成立
         */
        bool Wait(const unit::Time &time) const;

        /**
         * @brief 等待条件成立；当条件成立后，立即将值重置
         */
        bool WaitAndInit(bool value = false);

        /**
         * @brief 使条件满足，将释放所有正在等待条件满足的线程。
         */
        void Satisfy();

        /**
         * @brief 标识条件永远无法被满足，将释放所有正在等待条件满足的线程，并使他们返回 false.
         */
        void Fail();

        /**
         * @return 本条件的值
         */
        operator bool () const;   // NOLINT(google-explicit-constructor)

    private:
        mutable std::condition_variable _condition;
        mutable std::mutex              _mutex;
        bool                            _value  = false;   // 标识本条件的成立状态
        bool                            _failed = false;   // 标识本条件是否无法被满足
    };
}   // namespace poker::thread