//
// Created by zyk on 24-6-4.
//

#pragma once

#include <mutex>
#include <shared_mutex>

#include <poker/macro.h>

#include "./thread_lock/LockHelper.h"
#include "./thread_lock/LockImpl.h"
#include "./thread_lock/SharedLockHelper.h"
#include "./thread_lock/SharedLockImpl.h"


namespace poker::thread_lock
{
    /**
     * 以下的宏是用于多线程加锁的语法糖。
     * @example 继承一个可以被加锁的类
     *
     * class A : public poker::thread_lock::MutexLock
     * {
     * public:
     *     void f()
     *     {
     *         Synchronized(this)
     *         {
     *             // ...
     *         }
     *     }
     * };
     *
     * @example 修饰任意类型使之成为可以被加锁的类型
     *
     * MutexLockable(std::vector<int>) data;
     *
     * Locking(data)
     * {
     *     // ...
     * }
     *
     * Synchronized(&data)
     * {
     *     // ...
     * }
     */

/**
 * @brief 修饰某种类类型，为指定的锁类型
 */
#define Lockable(_LockType_, ...)                 \
    class : public __VA_ARGS__, public _LockType_ \
    {                                             \
    public:                                       \
        using TBase = __VA_ARGS__;                \
        using TBase::TBase;                       \
        using TBase::operator=;                   \
    }

/**
 * @brief 作用于锁对象，制造一个加锁的作用域
 * @example 无时限加锁
 * MutexLockable lock;
 *
 * Locking(lock)
 * {
 *     // ...
 * }
 *
 * @example 有时限加锁
 * Locking(lock, poker::unit::Second(1))
 * {
 *     // ...
 * }
 */
#define Locking(...) POKER_CONCAT(INNER_LOCKING_, POKER_COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

// 无时限加锁
#define INNER_LOCKING_1(_lock_) \
    if constexpr (poker::thread_lock::details::LockHelper POKER_COUNTER_UNIQUE(__lock__)(_lock_); true)

// 有时限加锁
#define INNER_LOCKING_2(_lock_, _time_)                                                      \
    if (poker::thread_lock::details::LockHelper POKER_LINE_UNIQUE(__lock__)(_lock_, _time_); \
        POKER_LINE_UNIQUE(__lock__).IsOwned())


/**
 * @brief 作用于共享锁对象，制造一个写者锁的作用域
 * @example 无时限写者锁
 * SharedMutexLockable lock;
 *
 * WriterLocking(lock)
 * {
 *     // ...
 * }
 *
 * @example 有时限写者锁
 * WriterLocking(lock, poker::unit::Second(1))
 * {
 *     // ...
 * }
 */
#define WriterLocking(...) Locking(__VA_ARGS__)


/**
 * @brief 作用于共享锁对象，制造一个读者锁的作用域
 * @example 无时限读者锁
 * SharedMutexLockable lock;
 *
 * ReaderLocking(lock)
 * {
 *     // ...
 * }
 *
 * @example 有时限读者锁
 * ReaderLocking(lock, poker::unit::Second(1))
 * {
 *     // ...
 * }
 */
#define ReaderLocking(...) POKER_CONCAT(INNER_READER_LOCKING_, POKER_COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

// 无时限读者锁
#define INNER_READER_LOCKING_1(_lock_) \
    if constexpr (poker::thread_lock::details::SharedLockHelper POKER_COUNTER_UNIQUE(__lock__)(_lock_); true)

// 有时限读者锁
#define INNER_READER_LOCKING_2(_lock_, _time_)                                                     \
    if (poker::thread_lock::details::SharedLockHelper POKER_LINE_UNIQUE(__lock__)(_lock_, _time_); \
        POKER_LINE_UNIQUE(__lock__).IsOwned())

/**
 * @brief 作用于锁对象指针，制造一个加锁的作用域
 * @example 无时限加锁
 * MutexLockable * lock_ptr;
 *
 * Synchronized(lock_ptr)
 * {
 *     // ...
 * }
 *
 * @example 有时限加锁
 * Synchronized(lock_ptr, poker::unit::Second(1))
 * {
 *     // ...
 * }
 */
#define Synchronized(...) POKER_CONCAT(INNER_SYNCHRONIZED_, POKER_COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

// 无时限加锁（用于指针）
#define INNER_SYNCHRONIZED_1(_lock_ptr_) Locking(*(_lock_ptr_))

// 有时限加锁（用于指针）
#define INNER_SYNCHRONIZED_2(_lock_ptr_, _time_) Locking(*(_lock_ptr_), _time_)

/**
 * @brief 作用于共享锁对象指针，制造一个写者锁的作用域
 * @example 无时限写者锁
 * SharedMutexLockable * lock_ptr;
 *
 * WriterSynchronized(lock_ptr)
 * {
 *     // ...
 * }
 *
 * @example 有时限写者锁
 * WriterSynchronized(lock_ptr, poker::unit::Second(1))
 * {
 *     // ...
 * }
 */
#define WriterSynchronized(...) Synchronized(__VA_ARGS__)

/**
 * @brief 作用于共享锁对象指针，制造一个读者锁的作用域
 * @example 无时限读者锁
 * SharedMutexLockable * lock_ptr;
 *
 * ReaderSynchronized(lock_ptr)
 * {
 *     // ...
 * }
 *
 * @example 有时限读者锁
 * ReaderSynchronized(lock_ptr, poker::unit::Second(1))
 * {
 *     // ...
 * }
 */
#define ReaderSynchronized(...) POKER_CONCAT(INNER_READER_SYNCHRONIZED_, POKER_COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

// 无时限加锁（用于指针）
#define INNER_READER_SYNCHRONIZED_1(_lock_ptr_) ReaderLocking(*(_lock_ptr_))

// 有时限加锁（用于指针）
#define INNER_READER_SYNCHRONIZED_2(_lock_ptr_, _time_) ReaderLocking(*(_lock_ptr_), _time_)


    using MutexLock       = details::LockImpl< std::timed_mutex >;
    using RecursiveLock   = details::LockImpl< std::recursive_timed_mutex >;
    using SharedMutexLock = details::SharedLockImpl< std::shared_timed_mutex >;

#define MutexLockable(...)       Lockable(poker::thread_lock::MutexLock, __VA_ARGS__)
#define RecursiveLockable(...)   Lockable(poker::thread_lock::RecursiveLock, __VA_ARGS__)
#define SharedMutexLockable(...) Lockable(poker::thread_lock::SharedMutexLock, __VA_ARGS__)
}   // namespace poker::thread_lock