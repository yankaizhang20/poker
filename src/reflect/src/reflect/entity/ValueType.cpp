//
// Created by zyk on 24-6-5.
//

#include "poker/reflect/entity/ValueType.h"


namespace poker::reflect
{
    std::string GetValueTypeName(ValueType type)
    {
        switch (type)
        {
#define STRING_CASE(_type_) \
    case ValueType::_type_: \
        return #_type_;

            POKER_VALUE_REFLECT_TYPE(STRING_CASE)

            default:
                poker_no_impl();
#undef STRING_CASE
        }
    }

    std::string GetFullValueTypeName(ValueType type)
    {
        return "poker::param::" + GetValueTypeName(type);
    }
}   // namespace poker::reflect