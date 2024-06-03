//
// Created by zyk on 24-5-31.
//

#pragma once

#include "./ValueType.h"


namespace poker::reflect::trait
{
    /**
     * @brief 识别 ValueType 枚举类型对应的真实类型
     */
    template < ValueType enum_type >
    struct value_impl
    {
    };

#define VALUE_IMPL_TRAIT(_type_)           \
    template <>                            \
    struct value_impl< ValueType::_type_ > \
    {                                      \
        using type = _type_;               \
    };

    POKER_VALUE_REFLECT_TYPE(VALUE_IMPL_TRAIT)

#undef VALUE_IMPL_TRAIT

    /**
     * @brief Value 便捷类型识别
     */
    template < ValueType enum_type >
    using value_t = typename value_impl< enum_type >::type;
}   // namespace poker::reflect::trait