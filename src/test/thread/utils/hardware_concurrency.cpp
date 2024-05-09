//
// Created by zyk on 24-3-29.
//

#include <iostream>
#include <thread>

int main()
{
    std::cout << "本机支持的最大系统线程数量" << std::thread::hardware_concurrency() << std::endl;

    return 0;
}