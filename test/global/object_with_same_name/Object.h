//
// Created by zyk on 24-4-19.
//

#pragma once

#include <iostream>
#include <string>

#define DEF_OBJECT(obj)                                \
    class Object                                       \
    {                                                  \
    public:                                            \
        Object()                                       \
        {                                              \
            std::cout << "construct obj" << std::endl; \
            p = new char[ 3 ];                         \
        }                                              \
                                                       \
        ~Object()                                      \
        {                                              \
            std::cout << "destruct obj" << std::endl;  \
            delete[] p;                                \
        }                                              \
                                                       \
    private:                                           \
        char *p = nullptr;                             \
    };
