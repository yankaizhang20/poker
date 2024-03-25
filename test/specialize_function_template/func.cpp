//
// Created by zyk on 24-3-8.
//
#include <iostream>
#include "./header.h"

// template <>
// std::string channel_name< MyChannel > = "name for MyChannel";

void func()
{
    channel_name< int >();
    channel_name< MyChannel >();
}