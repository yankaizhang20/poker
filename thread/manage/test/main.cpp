//
// Created by zyk on 24-1-15.
//
#include <iostream>
#include "manage.h"

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
        poker::thread::manage::ThreadAutoJoin t1(Func, 1, 1000);
    }

    std::cout << "线程结束" << std::endl;

    return 0;
}