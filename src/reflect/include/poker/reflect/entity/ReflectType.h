//
// Created by zyk on 24-5-30.
//

#pragma once

#include <poker/macro.h>


/**
 * @brief 便于扩展内部反射类型的工具
 */
#define POKER_ALL_REFLECT_TYPE(func) POKER_INVOKE(func, Value, Enum, Struct, Vector, List, Map)


/**
 * @brief 内部反射类型
 */
enum class ReflectType
{
    Undefined
#define DECLARE_ENUM(Type) , Type

    POKER_ALL_REFLECT_TYPE(DECLARE_ENUM)

#undef DECLARE_ENUM
};