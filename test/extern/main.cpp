//
// Created by zyk on 24-3-8.
//
#include<iostream>

extern int var;

extern void test_func();

void no_impl_func();

int main()
{
    std::cout<<var<<std::endl;
    test_func();
}