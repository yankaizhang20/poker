//
// Created by zyk on 24-6-3.
//

#pragma once

#include "./reflect/entity/TypeView.h"
#include "./reflect/entity/type_trait.h"



/**
 * @brief 实现类型反射
 * @example
 * struct Object
 * {
 *      int a;
 *      double b;
 * };
 *
 * POKER_REFLECT(Object, a, b)
 */
#define POKER_REFLECT(Type, ...)                           \
    namespace poker::reflect                               \
    {                                                      \
        template <>                                        \
        void Bind< Type >(Type & obj, TypeView &view)      \
        {                                                  \
            POKER_INVOKE(INNER_ADD_ONE_FIELD, __VA_ARGS__) \
        }                                                  \
    }

#define INNER_ADD_ONE_FIELD(_field_) view.AddField(#_field_, obj._field_);