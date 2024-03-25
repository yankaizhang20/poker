//
// Created by zyk on 24-3-8.
//
#include <iostream>
#include "./header.h"

// template <>
// std::string channel_name< MyChannel > = "name for MyChannel";

void func()
{
    channel_name< int >       n_int;
    channel_name< MyChannel > n_My;

    std::cout << n_My.value() << std::endl;
    std::cout << n_int.value() << std::endl;
}