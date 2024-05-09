//
// Created by zyk on 24-3-8.
//

#pragma once
#include <string>


struct MyChannel
{
};


template < class Channel >
struct channel_name
{
    std::string value()
    {
        return "default";
    }
};

template <>
struct channel_name< MyChannel >
{
    std::string value()
    {
        return "spec";
    }
};