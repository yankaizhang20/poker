//
// Created by zyk on 24-6-7.
//

#pragma once

#include <poker/databus.h>


namespace channel
{
    using MyTopicChannel = poker::databus::TopicChannel< bool >;

    using MyServiceChannel = poker::databus::ServiceChannel< int, bool >;
}   // namespace channel