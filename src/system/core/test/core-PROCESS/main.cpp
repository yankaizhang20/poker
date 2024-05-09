//
// Created by zyk on 24-5-9.
//

#include <iostream>
#include <poker/core.h>


int main()
{
    poker::Init();

    std::cout << "running!" << std::endl;

    poker::Yield();

    return 0;
}
