//
// Created by zyk on 24-5-9.
//

#include <iostream>

#include <poker/system.h>


class ModuleA : public poker::system::Modular< ModuleA >
{
public:
    void ModuleStart() override
    {
        std::cout << "ModuleA start!" << std::endl;
    }

    void ModuleShutdown() override
    {
        std::cout << "ModuleA end!" << std::endl;
    }

    size_t GetLaunchPriority() override
    {
        return 0;
    }
};

class ModuleB : public poker::system::Modular< ModuleB >
{
public:
    void ModuleStart() override
    {
        std::cout << "ModuleB start!" << std::endl;
    }

    void ModuleShutdown() override
    {
        std::cout << "ModuleB end!" << std::endl;
    }

    size_t GetLaunchPriority() override
    {
        return 1;
    }
};

POKER_NO_OPTIMIZED(ModuleA)
POKER_NO_OPTIMIZED(ModuleB)

int main()
{
    poker::system::Init();

    std::cout << "exit!" << std::endl;

    return 0;
}