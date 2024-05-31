//
// Created by zyk on 24-5-31.
//

#include "poker/reflect/entity/TypeView.h"


namespace poker::reflect
{
    ReflectType TypeView::GetType() const
    {
        return ReflectType(data_.index());
    }

#define DECLARE_FUNCS(Type)                          \
    bool TypeView::Is##Type() const                  \
    {                                                \
        return GetType() == ReflectType::Type;       \
    }                                                \
                                                     \
    TypeView::Type &TypeView::As##Type()             \
    {                                                \
        return std::get< Type >(data_);              \
    }                                                \
                                                     \
    const TypeView::Type &TypeView::As##Type() const \
    {                                                \
        return std::get< Type >(data_);              \
    }

    POKER_ALL_REFLECT_TYPE(DECLARE_FUNCS)
#undef DECLARE_FUNCS
}   // namespace poker::reflect