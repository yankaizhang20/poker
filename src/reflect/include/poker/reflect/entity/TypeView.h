//
// Created by zyk on 24-5-27.
//

#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <variant>

#include "./meta.h"


namespace poker::reflect
{
    class TypeView
    {
    public:
        /**
         * @brief 记录类型中的一个数据成员
         * @tparam T 数据成员类型
         * @param name 数据成员名
         * @param field 数据成员
         */
        template < class T >
        void AddField(const std::string &name, T &field)
        {
        }

    private:
        class Value
        {
        };

        class Enum
        {
        };

        class Struct
        {
        private:
            std::list< std::pair< std::string, TypeView > > field_;
        };

        class Vector
        {
        };

        class List
        {
        };

        class Map
        {
        };

    public:
#define DECLARE_FUNCS(Type) \
    bool Is##Type() const;  \
                            \
    Type &As##Type();       \
                            \
    const Type &As##Type() const;

        POKER_ALL_REFLECT_TYPE(DECLARE_FUNCS)
#undef DECLARE_FUNCS

    private:
#define DECLARE_TYPE(type) , type
        std::variant< std::nullptr_t POKER_ALL_REFLECT_TYPE(DECLARE_TYPE) > data_;
#undef DECLARE_TYPE
    };
}   // namespace poker::reflect