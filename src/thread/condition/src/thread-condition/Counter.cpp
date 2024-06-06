//
// Created by zyk on 24-6-6.
//

#include "poker/thread/condition/Counter.h"


namespace poker::thread
{
    Counter::Counter(std::size_t initial_count)
    {
        Reset(initial_count);
    }

    Counter::~Counter()
    {
        FailAll();
    }

    void Counter::Increase(std::size_t n)
    {
        std::unique_lock lock(_mutex);
        _count += n;
        _con.notify_all();
    }

    void Counter::Decrease(std::size_t n)
    {
        std::unique_lock lock(_mutex);

        if (_count < n)
            _count = 0;
        else
            _count -= n;

        _con.notify_all();
    }

    void Counter::IncreaseOne()
    {
        Increase(1);
    }

    void Counter::DecreaseOne()
    {
        Decrease(1);
    }

    void Counter::Reset(std::size_t initial_count)
    {
        std::unique_lock lock(_mutex);
        _failed = false;
        _count  = initial_count;
        _con.notify_all();
    }

    void Counter::FailAll()
    {
        std::unique_lock lock(_mutex);
        _failed = true;
        _con.notify_all();
    }

    bool Counter::WaitUntilLessThan(std::size_t n) const
    {
        return InnerWait([ this, n ]() { return _count < n; });
    }

    bool Counter::WaitUntilLessEqualThan(std::size_t n) const
    {
        return InnerWait([ this, n ]() { return _count <= n; });
    }

    bool Counter::WaitUntilEqual(std::size_t n) const
    {
        return InnerWait([ this, n ]() { return _count == n; });
    }

    bool Counter::WaitUntilLargerThan(std::size_t n) const
    {
        return InnerWait([ this, n ]() { return _count > n; });
    }

    bool Counter::WaitUntilLargerEqualThan(std::size_t n) const
    {
        return InnerWait([ this, n ]() { return _count >= n; });
    }

    Counter::operator std::size_t () const
    {
        std::unique_lock lock(_mutex);
        return _count;
    }

    bool Counter::InnerWait(const std::function< bool() > &condition) const
    {
        std::unique_lock lock(_mutex);

        while (not _failed and not condition())
            _con.wait(lock);

        return not _failed;
    }
}   // namespace poker::thread