//
// Created by zyk on 2024-6-4
//

#pragma once

#include "./LockImpl.h"


namespace poker::thread::details
{
    template<class TMutex>
    class SharedLockImpl : public LockImpl<TMutex>
    {
    public:
        void SharedLock() const
        {
            LockImpl<TMutex>::_mutex.lock_shared();
        }

        void UnSharedLock() const
        {
            LockImpl<TMutex>::_mutex.unlock_shared();
        }

        [[nodiscard]] bool TrySharedLock() const
        {
            return LockImpl<TMutex>::_mutex.try_lock_shared();
        }

        [[nodiscard]] bool TrySharedLockFor(const poker::unit::Time & time) const
        {
            return LockImpl<TMutex>::_mutex.try_lock_shared_until(poker::utils::TimePointFromNow(time));
        }
    };
}