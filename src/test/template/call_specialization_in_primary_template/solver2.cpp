//
// Created by zyk on 24-4-11.
//

#include <iostream>

#include "./entity.h"


template < class TSpecDerive >
struct BaseTrait
{
    using type = TSpecDerive;
};

template < template < typename > typename TDerive >
struct BaseTrait< TDerive< BaseA > >
{
    using type = BaseA;
};

template < template < typename > typename TDerive >
struct BaseTrait< TDerive< BaseB > >
{
    using type = BaseB;
};


// 在这里唯一定义对不同基类类型的处理
template < class TDerive >
void func_impl(TDerive &src, BaseA)
{
    std::cout << "process type of BaseA" << std::endl;
}

template < class TDerive >
void func_impl(TDerive &src, BaseB)
{
    std::cout << "process type of BaseB" << std::endl;
}



// 用于处理各种 Derive 类型的基类成员
template < class TDerive >
struct Helper
{
    static void func(TDerive &src)
    {
        func_impl(src, typename BaseTrait< TDerive >::type {});
    }
};


#include "./test.h"