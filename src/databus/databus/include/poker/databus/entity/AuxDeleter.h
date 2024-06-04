//
// Created by zyk on 24-6-4.
//

#pragma once

#include <functional>


namespace poker::databus
{
    struct AuxDeleter
    {
        std::function< void() > deleter_caller;
    };
}   // namespace poker::databus