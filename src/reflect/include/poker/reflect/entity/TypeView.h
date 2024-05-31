//
// Created by zyk on 24-5-27.
//

#pragma once

#include <list>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>

#include "./extern_impl.h"
#include "./meta.h"
#include "./type_trait.h"


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
            AsStruct().AddField(name, field);
        }

    private:
        struct Value
        {
            int *p_data;
        };

        struct Enum
        {
            int *p_data;
        };

        class Struct
        {
        public:
            template < class T >
            void AddField(const std::string &name, T &field)
            {
                field_.emplace_back(name);

                if constexpr (std::is_scalar_v< T >)
                {
                    Bind(field, field_.back().second.AsValue());
                }
                else if constexpr (std::is_enum_v< T >)
                {
                    Bind(field, field_.back().second.AsEnum());
                }
                else if constexpr (std::is_same_v< reflect_type_t< T >, trait::Vector >)
                {
                    Bind(field, field_.back().second.AsVector());
                }
                else if constexpr (std::is_same_v< reflect_type_t< T >, trait::List >)
                {
                    Bind(field, field_.back().second.AsList());
                }
                else if constexpr (std::is_same_v< reflect_type_t< T >, trait::Map >)
                {
                    Bind(field, field_.back().second.AsMap());
                }
                else
                {
                    Bind(field, field_.back().second.AsStruct());
                }
            }

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

    private:
        template < class T >
        static void Bind(T &obj, Value &view)
        {
            view.p_data = &obj;
        }

        template < class T >
        static void Bind(T &obj, Enum &view)
        {
            view.p_data = &obj;
        }

    public:
#define DECLARE_FUNCS(Type)       \
    bool        Is##Type() const; \
    Type       &As##Type();       \
    const Type &As##Type() const;

        POKER_ALL_REFLECT_TYPE(DECLARE_FUNCS)

#undef DECLARE_FUNCS

    private:
#define DECLARE_TYPE(type) , type
        std::variant< std::nullptr_t POKER_ALL_REFLECT_TYPE(DECLARE_TYPE) > data_;
#undef DECLARE_TYPE
    };
}   // namespace poker::reflect