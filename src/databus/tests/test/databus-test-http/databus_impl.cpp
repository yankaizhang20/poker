//
// Created by zyk on 24-6-6.
//

#include <poker/databus/http.h>
#include <poker/databus/null.h>

#include "channel.h"

// clang-format off
poker_topic_impl(null,channel::MyTopicChannel )
poker_service_impl(http, channel::MyServiceChannel)
        // clang-format on