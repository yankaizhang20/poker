//
// Created by zyk on 24-5-9.
//

#include <iostream>

#include <poker/system.h>


int main()
{
    poker::system::Init();

    std::cout << "running!" << std::endl;

    poker::system::Yield();

    return 0;
}
