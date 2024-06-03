//
// Created by zyk on 24-6-3.
//

#pragma once


namespace poker::reflect
{
/**
 * @brief 便于扩展 Value 类型的工具
 */
#define POKER_VALUE_REFLECT_TYPE(func)                                                                      \
    POKER_INVOKE(func, BasicInt8, BasicUInt8, BasicInt16, BasicUInt16, BasicInt32, BasicUInt32, BasicInt64, \
                 BasicUInt64, BasicFloat, BasicDouble, BasicBool, BasicChar, BasicString)

    /**
     * @brief Value 支持的所有类型
     */
    enum class ValueType
    {
        Undefined
#define DECLARE_ENUM(Type) , Type
        POKER_VALUE_REFLECT_TYPE(DECLARE_ENUM)
#undef DECLARE_ENUM
    };

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
}   // namespace poker::reflect