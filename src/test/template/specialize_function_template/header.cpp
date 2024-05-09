//
// Created by zyk on 24-3-8.
//
#include "header.h"

template <>
void channel_name< MyChannel >()
{
    std::cout << "spec" << std::endl;
}