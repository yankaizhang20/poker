//
// Created by zyk on 2024-6-6.
//

#include "poker/thread/condition/Semaphore.h"


namespace poker::thread
{
    Semaphore::Semaphore(long max_count)
    {
        InnerReset(max_count);
    }

    void Semaphore::SetMaxCount(long max_count)
    {
        std::unique_lock lock(_mutex);

        if (_max_count < max_count)
            _count += max_count - _max_count;

        _max_count = max_count;
        _con.notify_all();
    }

    bool Semaphore::Acquire()
    {
        std::unique_lock lock(_mutex);

        while (not _failed and _count == 0)
        {
            _con.wait(lock);
        }

        if (not _failed)
            --_count;

        return not _failed;
    }

    bool Semaphore::Acquire(const unit::Time &time)
    {
        auto tp = utils::TimePointFromNow(time);

        std::unique_lock lock(_mutex);

        while (not _failed and _count == 0)
        {
            if (_con.wait_until(lock, tp) == std::cv_status::timeout)
                return false;
        }

        if (not _failed)
            --_count;

        return not _failed;
    }

    bool Semaphore::TryAcquire()
    {
        std::unique_lock lock(_mutex);

        if (_failed)
            return false;

        if (_count != 0)
        {
            --_count;
            return true;
        }

        return false;
    }

    void Semaphore::Release()
    {
        std::unique_lock lock(_mutex);

        if (_max_count < 0 or _count < _max_count)
        {
            ++_count;
            _con.notify_one();
        }
    }

    void Semaphore::FailAll()
    {
        std::unique_lock lock(_mutex);

        _failed = true;
        _con.notify_all();
    }

    void Semaphore::Reset()
    {
        std::unique_lock lock(_mutex);
        InnerReset(_max_count);
    }

    void Semaphore::Reset(long max_count)
    {
        std::unique_lock lock(_mutex);
        InnerReset(max_count);
    }

    Semaphore::~Semaphore()
    {
        FailAll();
    }

    void Semaphore::InnerReset(long max_count)
    {
        _failed    = false;
        _max_count = max_count;
        _count     = std::max< long >(0, max_count);
        _con.notify_all();
    }
}   // namespace poker::thread
