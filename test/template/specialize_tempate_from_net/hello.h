//
// Created by zyk on 24-3-8.
//

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <iostream>

template <class T>
class Hello
{
public:
    void print_hello(T var);
};

template <class T>
void Hello<T>::print_hello(T var)
{
    std::cout << "Hello generic function " << var << "\n";
}

template <> //inline
void Hello<int>::print_hello(int var)
{
    std::cout << "Hello specialized function " << var << "\n";
}

#endif