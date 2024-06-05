//
// Created by zyk on 24-6-3.
//

#pragma once

#include "./reflect/entity/TypeView.h"


/**
 * @brief 实现类型反射
 * @example
 * struct Object
 * {
 *      int a;
 *      double b;
 * };
 *
 * POKER_REFLECT_TYPE(Object, a, b)
 */
#define POKER_REFLECT_TYPE(Type, ...)                                                     \
    template <>                                                                           \
    inline void poker::reflect::Bind< Type >(Type & obj, poker::reflect::TypeView & view) \
    {                                                                                     \
        POKER_INVOKE(INNER_ADD_ONE_FIELD, __VA_ARGS__)                                    \
    }

#define INNER_ADD_ONE_FIELD(_field_) view.AddField(#_field_, obj._field_);


/**
 * @brief 枚举类型进行可读化的绑定宏
 * @example
 *
 * enum MyEnum
 * {
 *     Item1, Item2
 * };
 *
 * POKER_REFLECT_ENUM(MyEnum, Item1, Item2)
 */
#define POKER_REFLECT_ENUM(Type, ...)                                                 \
    template <>                                                                       \
    inline const poker::reflect::EnumItemTable &poker::reflect::Enumerating< Type >() \
    {                                                                                 \
        using poker_reflect_enumerating_type_ = Type;                                 \
                                                                                      \
        static const poker::reflect::EnumItemTable &items = {                         \
                POKER_INVOKE(INNER_REFLECT_ENUMERATING_IMPL_ONE_ITEM, __VA_ARGS__)};  \
                                                                                      \
        return items;                                                                 \
    }

#define INNER_REFLECT_ENUMERATING_IMPL_ONE_ITEM(_item_) {#_item_, poker_reflect_enumerating_type_::_item_},