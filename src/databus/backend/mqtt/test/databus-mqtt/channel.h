//
// Created by zyk on 24-3-8.
//

#pragma once

#include <poker/databus.h>


namespace tag
{
    struct MyTag
    {
    };
}   // namespace tag

namespace channel
{
    using MyServiceChannel = poker::databus::XServiceChannel< int, int, tag::MyTag >;
}