//
// Created by poker on 2024-6-5
//

#pragma once

#include <optional>
#include <string>


namespace poker::databus::serialization
{
    /**
     * @brief 序列化指定数据类型，返回序列化结果。
     * @note 须由各个类型特化实现。
     */
    template < class T >
    extern std::string Encode(const T &data);

    /**
     * @brief 序列化指定数据类型，将结果写入到给定的字符串参数中。
     */
    template < class T >
    inline void Encode(const T &data, std::string &binary_data)
    {
        binary_data = Encode(data);
    }

    /**
     * @brief 尝试反序列化二进制数据，若成功，则返回指定类型的数据。
     */
    template < class T >
    extern std::optional< T > Decode(const std::string &binary_data);

    /**
     * @brief 反序列化指定数据，返回成功与否。
     */
    template < class T >
    inline bool Decode(T &result, const std::string &binary_data)
    {
        if (std::optional< T > opt = Decode< T >(binary_data); opt.has_value())
        {
            result = std::move(opt.value());
            return true;
        }

        return false;
    }

    /**
     * @brief 特化声明指定类型的序列化、反序列化 API
     */
#define POKER_SERIALIZATION_API(Type)             \
    template <>                                   \
    std::string Encode< Type >(const Type &data); \
                                                  \
    template <>                                   \
    std::optional< Type > Decode< Type >(const std::string &binary_data)

    /**
     * @brief 特化声明基于 proto 实现的指定类型的序列化、反序列化 API
     */
#define POKER_SERIALIZATION_API_BY_PROTO(OriginalType, ProtoType)                               \
    template <>                                                                                 \
    inline std::string Encode< OriginalType >(const OriginalType &data)                         \
    {                                                                                           \
        return proto::ToBinary< ProtoType, OriginalType >(data);                                \
    }                                                                                           \
                                                                                                \
    template <>                                                                                 \
    inline std::optional< OriginalType > Decode< OriginalType >(const std::string &binary_data) \
    {                                                                                           \
        if (ProtoType proto_data; proto_data.ParseFromString(binary_data))                      \
            return proto::From< OriginalType, ProtoType >(proto_data);                          \
        else                                                                                    \
            return {};                                                                          \
    }
}   // namespace poker::databus::serialization

/**
 * @brief 实现 protobuf 类型的 serialization 的实现。
 */
#define POKER_IMPL_PROTO_SERIALIZATION(ProtoType)                                         \
    template <>                                                                           \
    inline std::string Encode< ProtoType >(const ProtoType &data)                         \
    {                                                                                     \
        return data.SerializeAsString();                                                  \
    }                                                                                     \
                                                                                          \
    template <>                                                                           \
    inline std::optional< ProtoType > Decode< ProtoType >(const std::string &binary_data) \
    {                                                                                     \
        if (ProtoType proto_data; proto_data.ParseFromString(binary_data))                \
            return proto_data;                                                            \
        else                                                                              \
            return {};                                                                    \
    }

// 一些基础类型的序列化特化声明
namespace poker::databus::serialization
{
#define declare_serialization(Type)       \
    template <>                           \
    std::string Encode(const Type &data); \
                                          \
    template <>                           \
    std::optional< Type > Decode(const std::string &binary_data)

    declare_serialization(bool);
    declare_serialization(int);
    declare_serialization(long long);
    declare_serialization(std::string);
    declare_serialization(std::nullptr_t);

#undef declare_serialization
}   // namespace poker::databus::serialization
