//
// Created by zyk on 24-5-31.
//

#pragma once

#include <list>
#include <map>
#include <vector>

#include "./meta.h"


namespace poker::reflect::trait
{
#define DECLARE_META_TYPE(Type) \
    struct Type                 \
    {                           \
    };

    POKER_ALL_REFLECT_TYPE(DECLARE_META_TYPE)

#undef DECLARE_META_TYPE
}   // namespace poker::reflect::trait


namespace poker::reflect
{
    template < class T >
    struct reflect_type
    {
    };

    template < class T >
    struct reflect_type< std::vector< T > >
    {
        using type = trait::Vector;
    };

    template < class T >
    struct reflect_type< std::list< T > >
    {
        using type = trait::List;
    };

    template < class TKey, class TValue >
    struct reflect_type< std::map< TKey, TValue > >
    {
        using type = trait::Map;
    };

    template < class TKey, class TValue >
    struct reflect_type< std::unordered_map< TKey, TValue > >
    {
        using type = trait::Map;
    };

    template < class T >
    using reflect_type_t = typename reflect_type< T >::type;
}   // namespace poker::reflect