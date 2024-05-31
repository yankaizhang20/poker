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

    public:
        class Value
        {
        public:
            template < class T >
            explicit Value(T &obj) : data_(obj)
            {
            }

        private:
            int &data_;
        };

        class Enum
        {
        public:
            template < class T >
            explicit Enum(T &obj) : data_(obj)
            {
            }

        private:
            int &data_;
        };

        class Struct
        {
        public:
            template < class T >
            void AddField(const std::string &name, T &field)
            {
                field_.emplace_back(name);

                field_.back().second.Bind(field);
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

    public:
        template < class T >
        void Bind(T &obj)
        {
            if constexpr (std::is_scalar_v< T >)
            {
                data_.emplace< Value >(obj);
            }
            else if constexpr (std::is_enum_v< T >)
            {
                data_.emplace< Enum >(obj);
            }
            else
            {
                data_.emplace< Struct >();

                // 调用用户提供的反射函数
                reflect::Bind(obj, *this);
            }
        }

        template < class T >
        void Bind(std::vector< T > &obj)
        {
            data_.emplace< Vector >(obj);
        }

        template < class T >
        void Bind(std::list< T > &obj)
        {
            data_.emplace< List >(obj);
        }

        template < class TKey, class TValue >
        void Bind(std::map< TKey, TValue > &obj)
        {
            data_.emplace< Map >(obj);
        }

    public:
        ReflectType GetType() const;

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