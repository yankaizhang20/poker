//
// Created by zyk on 24-6-6.
//

#pragma once

#include <string>
#include <utility>

#include <poker/macro.h>


namespace poker::databus::http
{
#define POKER_DATABUS_ALL_HTTP_METHOD(_FUNC_) \
    POKER_INVOKE(_FUNC_, Get)                 \
    POKER_INVOKE(_FUNC_, Post)                \
    POKER_INVOKE(_FUNC_, Patch)               \
    POKER_INVOKE(_FUNC_, Delete)              \
    POKER_INVOKE(_FUNC_, Options)


    enum class Method
    {
#define HTTP_METHOD_ENUM(_TYPE_) _TYPE_,
        POKER_DATABUS_ALL_HTTP_METHOD(HTTP_METHOD_ENUM)
#undef HTTP_METHOD_ENUM
    };

#define HTTP_METHOD_DEFINE(_TYPE_) \
    struct _TYPE_                  \
    {                              \
        static std::string name()  \
        {                          \
            return #_TYPE_;        \
        }                          \
    };

    POKER_DATABUS_ALL_HTTP_METHOD(HTTP_METHOD_DEFINE)

#undef HTTP_METHOD_DEFINE

    namespace trait
    {
        template < Method method >
        struct HttpMethod
        {
        };

#define HTTP_METHOD_TRAIT(_TYPE_)       \
    template <>                         \
    struct HttpMethod< Method::_TYPE_ > \
    {                                   \
        using type = _TYPE_;            \
    };
#undef HTTP_METHOD_TRAIT

    }   // namespace trait

}   // namespace poker::databus::http


namespace poker::databus
{
    struct ChannelConfigBase
    {
        std::string channel_name;

        explicit ChannelConfigBase(std::string name) : channel_name(std::move(name))
        {
        }

        operator std::string () const
        {
            return channel_name;
        }
    };

    struct ChannelConfigHttp : public ChannelConfigBase
    {
        std::string ip;
        std::size_t port = -1;
        std::string request_target;

        // 默认使用 Post
        http::Method method = http::Method::Post;

        ChannelConfigHttp(const std::string &channel_name, std::string ip, size_t port, std::string requestTarget) :
            ChannelConfigBase(channel_name), ip(std::move(ip)), port(port), request_target(std::move(requestTarget))
        {
        }
    };

    struct ChannelConfig : public ChannelConfigHttp
    {
        ChannelConfig(const std::string &channelName, const std::string &ip, size_t port,
                      const std::string &requestTarget) :
            ChannelConfigHttp(channelName, ip, port, requestTarget)
        {
        }
    };
}   // namespace poker::databus