//
// Created by zyk on 24-4-10.
//

#include <iostream>

#include "./entity.h"


// 用于处理各种 Derive 类型的基类成员
template < class TDerive >
struct Helper
{
    template < class TFakeDerive >
    struct Dummy : TFakeDerive
    {
    };

    static void func(TDerive &src)
    {
        Dummy< TDerive > dummy;

        Helper< Dummy< TDerive > >::func(dummy);

        src = dummy;
    }
};

// 在这里唯一定义对不同基类类型的处理
template < template < typename > typename TDerive >
struct Helper< TDerive< BaseA > >
{
    static void func(TDerive< BaseA > &src)
    {
        std::cout << "process type of BaseA" << std::endl;
    }
};

template < template < typename > typename TDerive >
struct Helper< TDerive< BaseB > >
{
    static void func(TDerive< BaseB > &src)
    {
        std::cout << "process type of BaseB" << std::endl;
    }
};


#include "./test.h"