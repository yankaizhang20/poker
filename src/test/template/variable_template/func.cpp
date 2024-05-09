//
// Created by zyk on 24-3-8.
//
#include <iostream>
#include "./header.h"

// template <>
// std::string channel_name< MyChannel > = "name for MyChannel";

void func()
{
    std::cout << channel_name< MyChannel > << std::endl;
}