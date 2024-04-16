//
// Created by zyk on 24-4-16.
//

#include <iostream>
#include <string>

class Object
{
public:
    Object(std::string &&str)
    {
        std::cout << str << " object initialization" << std::endl;
    }
};

Object        object("global");
static Object object1("static");

bool Func(std::string &&str)
{
    std::cout << str << " object initialization" << std::endl;
    return true;
}

bool object2 = Func("from func");

// Func("from func");

int main()
{
    std::cout << "first instruction in main" << std::endl;
}