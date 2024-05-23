//
// Created by zyk on 24-1-15.
//
#include <iostream>
#include "poker/thread/manage.h"

void Func(int begin, int end)
{
    for (int i = begin; i != end; i++)
    {
        std::cout << i << std::endl;
    }
}


int main()
{
    {
        poker::thread::Thread t1(poker::thread::Retrieve::Join, Func, 1, 1'000);
    }

    std::cout << "线程结束" << std::endl;

    return 0;
}