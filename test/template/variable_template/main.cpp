//
// Created by zyk on 24-3-8.
//
#include <iostream>
#include "header.h"

void func();


template <>
inline std::string channel_name<MyChannel> = "MainMyChannel";

int main()
{
    channel_name< MyChannel > = "MyChannel";

    std::cout << channel_name< MyChannel > << std::endl;

    func();
}