//
// Created by zyk on 24-6-6.
//

#pragma once

#include <vector>

#include <poker/databus.h>


namespace test
{
    struct MyType1
    {
        int    a = 30;
        bool   b = true;
        double c = -1.0;
    };

    struct MyType2
    {
        std::vector< double > data;
    };
}   // namespace test

namespace channel
{
    using MyTopicChannel = poker::databus::TopicChannel< test::MyType1 >;

    using MyServiceChannel = poker::databus::ServiceChannel< test::MyType1, test::MyType2 >;
}   // namespace channel