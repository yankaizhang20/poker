#pragma once

#include <optional>
#include <string>
#include <google/protobuf/any.pb.h>

#include <poker/macro.h>


namespace poker::proto
{
    template < class TProtoType, class TOriginalType >
    extern TProtoType To(const TOriginalType &src);

    template < class TOriginalType, class TProtoType >
    extern TOriginalType From(const TProtoType &src);

    template < class TProtoType, class TOriginalType >
    inline std::string ToBinary(const TOriginalType &src)
    {
        return To< TProtoType, TOriginalType >(src).SerializeAsString();
    }

    template < class TProtoType, class TOriginalType >
    void ToAny(const TOriginalType &src, google::protobuf::Any &dst)
    {
        dst.PackFrom(To< TProtoType >(src));
    }

    template < class TProtoType, class TOriginalType >
    void ToAny(const TOriginalType &src, google::protobuf::Any *dst)
    {
        ToAny< TProtoType >(src, *dst);
    }

    template < class TOriginalType, class TProtoType >
    TOriginalType FromAny(const google::protobuf::Any &src_any)
    {
        TProtoType src;
        if (not src_any.UnpackTo(&src))
            poker_no_impl();
        return From< TOriginalType >(src);
    }

    template < class TProtoType, class TOriginalType >
    void FromAny(TOriginalType &dst, const google::protobuf::Any &src_any)
    {
        TProtoType src;
        if (not src_any.UnpackTo(&src))
            poker_no_impl();
        dst = From< TOriginalType >(src);
    }

    /**
     * @brief 创建指定数据类型的 proto 接口
     */
#define ARES_PROTO_API(OriginalType, ProtoType)                               \
    void To(const OriginalType &src, ProtoType &dst);                         \
                                                                              \
    inline void To(const OriginalType &src, ProtoType *dst)                   \
    {                                                                         \
        To(src, *dst);                                                        \
    }                                                                         \
                                                                              \
    template <>                                                               \
    inline ProtoType To< ProtoType, OriginalType >(const OriginalType &src)   \
    {                                                                         \
        ProtoType dst;                                                        \
        To(src, dst);                                                         \
        return dst;                                                           \
    }                                                                         \
                                                                              \
    void From(OriginalType &dst, const ProtoType &src);                       \
                                                                              \
    template <>                                                               \
    inline OriginalType From< OriginalType, ProtoType >(const ProtoType &src) \
    {                                                                         \
        OriginalType dst;                                                     \
        From(dst, src);                                                       \
        return dst;                                                           \
    }
}   // namespace poker::proto
