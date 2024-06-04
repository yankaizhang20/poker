//
// Created by poker on 2024-6-5
//

#include <poker/serialization.h>
#include <poker/serialization/basic.pb.h>


namespace poker::databus::serialization
{
#define implement_serialization(OriginalType, ProtoType)                \
    template<>                                                          \
    std::string Encode(const OriginalType & data)                       \
    {                                                                   \
        proto::ProtoType x;                                             \
        x.set_data(data);                                               \
        return x.SerializeAsString();                                   \
    }                                                                   \
                                                                        \
    template<>                                                          \
    std::optional<OriginalType> Decode(const std::string & binary_data) \
    {                                                                   \
        proto::ProtoType x;                                             \
        if (x.ParseFromString(binary_data))                             \
            return x.data();                                            \
        else                                                            \
            return {};                                                  \
    }

    implement_serialization(bool,        Bool)
    implement_serialization(int,         Int32)
    implement_serialization(long long,   Int64)
    implement_serialization(std::string, String)

#undef implement_serialization
}

namespace poker::databus::serialization
{
    template<>
    std::string Encode(const std::nullptr_t &)
    {
        return {};
    }

    template<>
    std::optional<std::nullptr_t> Decode(const std::string &)
    {
        return nullptr;
    }
}
