//
// Created by zyk on 24-6-6.
//

#include <poker/databus/this_process.h>

#include "channel.h"


// clang-format off
poker_topic_impl(this_process, channel::MyTopicChannel)

poker_service_impl(this_process, channel::MyServiceChannel)
        // clang-format on