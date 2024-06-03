//
// Created by zyk on 24-5-27.
//

#pragma once

#include <list>
#include <map>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

#include "./EnumView.h"
#include "./ReflectType.h"
#include "./ValueType.h"
#include "./extern_impl.h"
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
#define DECLARE_CONSTRUCT(Type)                                     \
    explicit Value(Type &obj) : type_(ValueType::Type), data_(&obj) \
    {                                                               \
    }

            POKER_VALUE_REFLECT_TYPE(DECLARE_CONSTRUCT)

#undef DECLARE_CONSTRUCT

        public:
#define AS_IMPLEMENTATION(_type_)                              \
    auto &As##_type_()                                         \
    {                                                          \
        return *(trait::value_t< ValueType::_type_ > *) data_; \
    }

            POKER_VALUE_REFLECT_TYPE(AS_IMPLEMENTATION)

#undef AS_IMPLEMENTATION

            ValueType GetType()
            {
                return type_;
            }

        private:
            // 记录类型信息
            ValueType type_;

            // 记录数据
            void *data_;
        };

        class Enum
        {
        public:
            template < class T >
            explicit Enum(T &obj) : data_(&obj)
            {
            }

        private:
            void *data_;
        };

        class Struct
        {
        public:
            template < class T >
            void AddField(const std::string &name, T &field)
            {
                field_.emplace_back(name, TypeView());
                field_.back().second.Bind(field);
            }

        private:
            std::list< std::pair< std::string, TypeView > > field_;
        };

        class Vector
        {
        public:
            template < class T >
            explicit Vector(T &obj)
            {
                for (auto &entry : obj)
                {
                    data_.emplace_back();
                    data_.back().Bind(entry);
                }
            }

        private:
            std::vector< TypeView > data_;
        };

        class List
        {
        public:
            template < class T >
            explicit List(T &obj)
            {
                for (auto &entry : obj)
                {
                    data_.emplace_back();
                    data_.back().Bind(entry);
                }
            }

        private:
            std::vector< TypeView > data_;
        };

        class Map
        {
        public:
            template < class T >
            explicit Map(T &obj)
            {
                for (auto &[ key, value ] : obj)
                {
                    data_.emplace_back(TypeView(), TypeView());

                    data_.back().first.Bind(key);
                    data_.back().second.Bind(value);
                }
            }

        private:
            std::list< std::pair< TypeView, TypeView > > data_;
        };

    public:
        template < class T >
        void Bind(T &obj)
        {
            if constexpr (std::is_arithmetic_v< T >)
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