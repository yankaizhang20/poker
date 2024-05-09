//
// Created by zyk on 24-3-8.
//

#pragma once
#include <iostream>
#include <string>


struct MyChannel
{
};


template < class Channel >
void channel_name()
{
    std::cout << "default" << std::endl;
}

template <>
void channel_name< MyChannel >();