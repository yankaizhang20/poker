//
// Created by zyk on 24-5-9.
//

#include <iostream>
#include <poker/core.h>

class ModuleA : public poker::AutoComponent< ModuleA >
{
public:
    void Setup() override
    {
        std::cout << "ModuleA start!" << std::endl;
    }

    void ShutDown() override
    {
        std::cout << "ModuleA end!" << std::endl;
    }

    size_t GetLaunchPriority() override
    {
        return 0;
    }
};

class ModuleB : public poker::AutoComponent< ModuleB >
{
public:
    void Setup() override
    {
        std::cout << "ModuleB start!" << std::endl;
    }

    void ShutDown() override
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
    poker::Init();

    std::cout << "exit!" << std::endl;

    return 0;
}