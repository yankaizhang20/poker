//
// Created by zyk on 24-5-30.
//

#pragma once

#include <poker/macro.h>


#define POKER_ALL_REFLECT_TYPE(func) POKER_INVOKE(func, Value, Enum, Struct, Vector, List, Map)


enum class ReflectType
{
    Undefined
#define DECLARE_ENUM(Type) , Type

    POKER_ALL_REFLECT_TYPE(DECLARE_ENUM)

#undef DECLARE_ENUM
};