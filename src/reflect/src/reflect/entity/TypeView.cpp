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


namespace poker::reflect
{
    TypeView &TypeView::Vector::Add()
    {
        adder_(data_.emplace_back());

        return data_.back();
    }

    void TypeView::Vector::ClearAndReserve(std::size_t size)
    {
        // 原数据
        cleaner_();
        reserver_(size);

        // 反射数据
        data_.clear();
        data_.reserve(size);
    }

    TypeView &TypeView::List::Add()
    {
        adder_(data_.emplace_back());

        return data_.back();
    }

    void TypeView::List::Clear()
    {
        cleaner_();

        data_.clear();
    }

    TypeView &TypeView::Map::Add(const std::string &key)
    {
        adder_(key, data_[ key ]);
        return data_[ key ];
    }

    void TypeView::Map::Clear()
    {
        cleaner_();
        data_.clear();
    }
}   // namespace poker::reflect