//
// Created by zyk on 2024-6-4
//

#pragma once

#include <atomic>
#include <chrono>

#include <poker/unit.h>


namespace poker::thread_lock::details
{
    template < class TMutex >
    class LockImpl
    {
    public:
        virtual ~LockImpl()
        {
            FreeSelf();
        }

        void Lock() const
        {
            _mutex.lock();
        }

        void UnLock() const
        {
            _mutex.unlock();
        }

        bool TryLock() const
        {
            return _mutex.try_lock();
        }

        bool TryLockFor(const poker::unit::Time &time) const
        {
            return _mutex.try_lock_until(poker::utils::TimePointFromNow(time));
        }

    protected:
        void FreeSelf()
        {
            if (_mutex.try_lock())
                UnLock();
        }

    protected:
        mutable TMutex _mutex;
    };
}   // namespace poker::thread_lock::details