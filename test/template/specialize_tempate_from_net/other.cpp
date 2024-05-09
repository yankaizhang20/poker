//
// Created by zyk on 24-3-8.
//
#include "hello.h"

void other_func()
{
    Hello<char> hc;
    Hello<int> hi;

    hc.print_hello('a');
    hi.print_hello(1);
}