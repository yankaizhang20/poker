//
// Created by zyk on 24-6-3.
//

#pragma once

#include "poker/unit.h"


namespace poker::reflect
{
/**
 * @brief Value 基础类型
 */
#define POKER_BASIC_VALUE_REFLECT_TYPE(_func_)                                                                \
    POKER_INVOKE(_func_, BasicInt8, BasicUInt8, BasicInt16, BasicUInt16, BasicInt32, BasicUInt32, BasicInt64, \
                 BasicUInt64, BasicFloat, BasicDouble, BasicBool, BasicChar, BasicString)

    using BasicBool   = bool;
    using BasicChar   = char;
    using BasicInt8   = int8_t;
    using BasicUInt8  = uint8_t;
    using BasicInt16  = int16_t;
    using BasicUInt16 = uint16_t;
    using BasicInt32  = int32_t;
    using BasicUInt32 = uint32_t;
    using BasicInt64  = int64_t;
    using BasicUInt64 = uint64_t;
    using BasicFloat  = float;
    using BasicDouble = double;
    using BasicString = std::string;

/**
 * @brief Value 特殊类型
 */
#define POKER_SPECIFIC_VALUE_REFLECT_TYPE(_func_) POKER_INVOKE(_func_, Angle, Distance, Time, Velocity, Weight)

    using Angle    = unit::Angle;
    using Distance = unit::Distance;
    using Time     = unit::Time;
    using Velocity = unit::Velocity;
    using Weight   = unit::Weight;

/**
 * @brief 所有 Value 类型
 */
#define POKER_VALUE_REFLECT_TYPE(_func_)   \
    POKER_BASIC_VALUE_REFLECT_TYPE(_func_) \
    POKER_SPECIFIC_VALUE_REFLECT_TYPE(_func_)


    /**
     * @brief Value 支持的所有类型
     */
    enum class ValueType
    {
        Undefined
#define DECLARE_ENUM(_Type_) , _Type_
        POKER_VALUE_REFLECT_TYPE(DECLARE_ENUM)
#undef DECLARE_ENUM
    };

    /**
     * @return 获取数据类型的名字，可用于可视化
     */
    std::string GetValueTypeName(ValueType type);

    /**
     * @return 获取带命名空间的数据类型名字
     */
    std::string GetFullValueTypeName(ValueType type);
}   // namespace poker::reflect