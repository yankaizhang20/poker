//
// Created by zyk on 24-3-8.
//
#include <iostream>
#include <string>

struct Type
{
    int         age  = 0;
    std::string name = "default";

    operator std::string ()
    {
        return name;
    }
};

std::string func(std::string name)
{
    std::cout << name;
    return name;
}

std::string func(Type type)
{
    std::cout << "really " << type.name;
    return type.name;
}

int main()
{
    Type test;
    //    std::cout << test << std::endl;
    std::cout << func(test);
}