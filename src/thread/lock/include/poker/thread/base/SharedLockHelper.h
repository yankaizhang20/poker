//
// Created by zyk on 2024-6-4
//

#pragma once

#include <poker/unit/Time.h>


namespace poker::thread::details
{
    template<class TLock>
    class SharedLockHelper
    {
    public:
        SharedLockHelper(const TLock & m)
            : _lock(m)
        {
            _lock.SharedLock();
            _owns = true;
        }

        SharedLockHelper(const TLock & m, const poker::unit::Time & time)
            : _lock(m)
        {
            _owns = _lock.TryLockFor(time);
        }

        [[nodiscard]] bool IsOwned() const
        {
            return _owns;
        }

        ~SharedLockHelper()
        {
            _lock.UnSharedLock();
        }

    private:
        const TLock & _lock;
        bool          _owns = false;
    };
}