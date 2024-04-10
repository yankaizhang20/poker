//
// Created by zyk on 24-4-10.
//

#pragma once

// 以下为业务数据定义

struct BaseA
{
};

struct BaseB
{
};

// 正常的业务类
template < class TBase >
struct Derive1
{
};

template < class TBase >
struct Derive2
{
};

// 模板类型下降为非模板类型
template < class TBase >
using SpecialDerive = TBase;