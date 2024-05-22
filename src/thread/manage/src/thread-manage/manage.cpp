//
// Created by zyk on 24-4-15.
//
#include <iostream>
#include "poker/thread/hierarchical_mutex.h"

namespace poker::thread::manage
{
    void Test()
    {
        lock::TestHierarchical();
        std::cout << "just for test" << std::endl;
    }
}   // namespace poker::thread::manage