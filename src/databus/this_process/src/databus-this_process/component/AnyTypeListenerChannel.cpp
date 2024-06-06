//
// Created by zyk on 24-6-6.
//

#include "poker/databus/this_process/component/AnyTypeListenerChannel.h"


namespace poker::databus
{
    std::atomic_size_t AnyTypeListenerChannel::_callback_unique_number = 0;

    bool AnyTypeListenerChannel::IsInitialized() const
    {
        return _core.has_value();
    }

    void AnyTypeListenerChannel::RemoveCallback(std::size_t callback_number)
    {
        if (_remove_callback_caller)
            _remove_callback_caller(callback_number);
    }

    void AnyTypeListenerChannel::Wait() const
    {
        if (_wait_caller)
            _wait_caller();
    }

    bool AnyTypeListenerChannel::Empty() const
    {
        if (_empty_caller)
            return _empty_caller();
        else
            return false;
    }
}   // namespace poker::databus