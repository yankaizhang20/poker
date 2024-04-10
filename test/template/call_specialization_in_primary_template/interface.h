//
// Created by zyk on 24-4-10.
//

#pragma once

// 用于处理各种 Derive 类型的基类成员, 要求实现为基类中的所有数据成员的值 +1
template < class TDerive >
struct Helper;

// note:要求处理基类成员的代码只能有一处定义，不能为特定派生类特化实现
