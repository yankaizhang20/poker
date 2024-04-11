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
template < class TBase, class TDerive >
struct HelperImpl
{
    static void func_impl(TDerive &src);
};

template < class TDerive >
struct HelperImpl< BaseA, TDerive >
{
    static void func_impl(TDerive &src)
    {
        std::cout << "process type of BaseA" << std::endl;
    }
};

template < class TDerive >
struct HelperImpl< BaseB, TDerive >
{
    static void func_impl(TDerive &src)
    {
        std::cout << "process type of BaseB" << std::endl;
    }
};


// 用于处理各种 Derive 类型的基类成员
template < class TDerive >
struct Helper
{
    static void func(TDerive &src)
    {
        HelperImpl< typename BaseTrait< TDerive >::type, TDerive >::func_impl(src);
    }
};


#include "./test.h"