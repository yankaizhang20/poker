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
    }                                                          \
    const auto &As##_type_() const                             \
    {                                                          \
        return const_cast< Value & >(*this).As##_type_();      \
    }

            POKER_VALUE_REFLECT_TYPE(AS_IMPLEMENTATION)

#undef AS_IMPLEMENTATION

            ValueType GetType() const
            {
                return type_;
            }

        private:
            // 记录类型信息
            ValueType type_;

            // 记录数据
            void *data_;
        };

        using Enum = EnumView;

        class Struct
        {
        public:
            template < class T >
            void AddField(const std::string &name, T &field)
            {
                field_.emplace_back(name, TypeView());
                field_.back().second.Bind(field);
            }

        public:
            auto begin()
            {
                return field_.begin();
            }

            auto end()
            {
                return field_.end();
            }

            auto begin() const
            {
                return field_.begin();
            }

            auto end() const
            {
                return field_.end();
            }

        private:
            std::list< std::pair< std::string, TypeView > > field_;
        };

        class Vector
        {
        public:
            using Adder    = std::function< void(TypeView &) >;
            using Cleaner  = std::function< void() >;
            using Reserver = std::function< void(std::size_t) >;

        public:
            template < class T >
            explicit Vector(T &obj)
            {
                adder_ = [ &obj ](TypeView &parameter) { parameter.Bind(obj.emplace_back()); };

                cleaner_ = [ &obj ]() { obj.clear(); };

                reserver_ = [ &obj ](std::size_t size) { obj.reserve(size); };

                for (auto &entry : obj)
                {
                    data_.emplace_back().Bind(entry);
                }
            }

            /**
             * @brief 添加新参数元素，也将修改到被绑定的容器上。
             */
            TypeView &Add();

            /**
             * @brief 清空元素，并改变容量
             * @param size
             */
            void ClearAndReserve(std::size_t size);

            auto begin()
            {
                return data_.begin();
            }

            auto end()
            {
                return data_.end();
            }

            auto begin() const
            {
                return data_.cbegin();
            }

            auto end() const
            {
                return data_.cend();
            }

        private:
            // 原数据操作器
            Adder    adder_;
            Cleaner  cleaner_;
            Reserver reserver_;

        private:
            std::vector< TypeView > data_;
        };

        class List
        {
        public:
            using Adder   = std::function< void(TypeView &) >;
            using Cleaner = std::function< void() >;

        public:
            template < class T >
            explicit List(T &obj)
            {
                adder_ = [ &obj ](TypeView &parameter) { parameter.Bind(obj.emplace_back()); };

                cleaner_ = [ &obj ]() { obj.clear(); };

                for (auto &entry : obj)
                {
                    data_.emplace_back().Bind(entry);
                }
            }

        public:
            /**
             * @brief 添加新参数元素，也将修改到被绑定的容器上。
             * @return 新增的参数对象
             */
            TypeView &Add();

            /**
             * @brief 清除所有参数元素，同时也将被绑定的容器清空。
             */
            void Clear();

            auto begin()
            {
                return data_.begin();
            }

            auto end()
            {
                return data_.end();
            }

            auto begin() const
            {
                return data_.cbegin();
            }

            auto end() const
            {
                return data_.cend();
            }

        private:
            // 原数据操作器
            Adder   adder_;
            Cleaner cleaner_;

        private:
            std::vector< TypeView > data_;
        };

        class Map
        {
        public:
            using Adder   = std::function< void(const std::string &, TypeView &) >;
            using Cleaner = std::function< void() >;

        public:
            template < class T >
            explicit Map(T &obj)
            {
                adder_ = [ &obj ](const std::string &key, TypeView &parameter) { parameter.Bind(obj[ key ]); };

                cleaner_ = [ &obj ]() { obj.clear(); };

                for (auto &[ key, value ] : obj)
                {
                    data_[ key ].Bind(value);
                }
            }

        public:
            /**
             * @brief 添加新参数元素，也将修改到被绑定的容器上。
             */
            TypeView &Add(const std::string &key);

            /**
             * @brief 清除所有参数元素，同时也将被绑定的容器清空。
             */
            void Clear();

            auto begin()
            {
                return data_.begin();
            }

            auto end()
            {
                return data_.end();
            }

            auto begin() const
            {
                return data_.cbegin();
            }

            auto end() const
            {
                return data_.cend();
            }

        private:
            // 元素操作器
            Adder   adder_;
            Cleaner cleaner_;

        private:
            std::map< std::string, TypeView > data_;
        };

    public:
        template < class T >
        void Bind(T &obj)
        {
            if constexpr (trait::ReflectValueClass< T >::enable)
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