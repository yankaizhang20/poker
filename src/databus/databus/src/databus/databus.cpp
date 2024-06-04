//
// Created by zyk on 24-6-4.
//

#include <poker/databus.h>


namespace poker::databus
{
    Aux &Aux::operator+= (AuxDeleter x)
    {
        if (x.deleter_caller != nullptr)
        {
            Synchronized(this) _data.push_back(std::move(x));
        }

        return *this;
    }

    void Aux::Offline()
    {
        for (auto &x : _data)
        {
            x.deleter_caller();
        }

        _data.clear();
    }

    Aux::~Aux()
    {
        Offline();
    }
}   // namespace poker::databus