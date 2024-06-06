//
// Created by zyk on 24-6-5.
//

#include "poker/thread/condition/Condition.h"


namespace poker::thread
{
    Condition::Condition(bool value) : _value(value)
    {
    }

    Condition::~Condition()
    {
        Fail();
    }

    void Condition::Init(bool value)
    {
        std::unique_lock lock(_mutex);
        _value  = value;
        _failed = false;
    }

    bool Condition::Wait() const
    {
        std::unique_lock lock(_mutex);

        while (not _failed and not _value)
        {
            _condition.wait(lock);
        }

        return _value;
    }

    bool Condition::Wait(const unit::Time &time) const
    {
        auto tp = utils::TimePointFromNow(time);

        std::unique_lock lock(_mutex);

        while (not _failed and not _value)
        {
            if (_condition.wait_until(lock, tp) == std::cv_status::timeout)
                break;
        }

        return _value;
    }

    bool Condition::WaitAndInit(bool value)
    {
        std::unique_lock lock(_mutex);

        while (not _failed and not _value)
        {
            _condition.wait(lock);
        }

        if (_value)
        {
            _value = value;
            return true;
        }
        else
            return false;
    }

    void Condition::Satisfy()
    {
        std::unique_lock lock(_mutex);
        _value  = true;
        _failed = false;
        _condition.notify_all();
    }

    void Condition::Fail()
    {
        std::unique_lock lock(_mutex);
        _value  = false;
        _failed = true;
        _condition.notify_all();
    }

    Condition::operator bool () const
    {
        std::unique_lock lock(_mutex);
        return _value;
    }
}   // namespace poker::thread