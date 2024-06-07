//
// Created by zyk on 24-6-4.
//

#pragma once

#include <optional>

#include "./entity/AuxDeleter.h"
#include "./entity/ChannelConfig.h"
#include "./entity/typedef.h"


namespace poker::databus_impl
{
    using AuxDeleter = databus::AuxDeleter;

    template < class Message >
    using Handler = databus::Handler< Message >;

    template < class Request, class Response >
    using Server = databus::Server< Request, Response >;

    /**
     * @brief 通道配置
     */
    template < class Channel >
    databus::ChannelConfig channel_config(typeid(Channel).name(), "localhost", 65'534, typeid(Channel).name());

    /**
     * @brief 通过指定字符串名称的通道，发送指定类型的数据
     */
    template < class T, class Tag = void >
    extern void Send(const databus::ChannelConfig &channel, const T &data);

    /**
     * @brief 订阅指定字符串名称的通道，监听接收指定类型的数据
     */
    template < class T, class Tag = void >
    extern AuxDeleter Listen(const databus::ChannelConfig &channel, const Handler< T > &handler);

    /**
     * @brief 调用指定通道中的服务过程，发出请求数据，并得到响应数据。
     */
    template < class Request, class Response, class Tag = void >
    extern std::optional< Response > Call(const databus::ChannelConfig &channel, const Request &req);

    /**
     * @brief 在指定通道中，注册一个服务过程。
     */
    template < class Request, class Response, class Tag = void >
    extern AuxDeleter Serve(const databus::ChannelConfig &channel, const Server< Request, Response > &server);

    /**
     * @brief 关闭指定字符串名称的通道；依然携带类型标签，是为了实现不同标签可以有不同的
     */
    template < class Channel >
    extern void Offline(const databus::ChannelConfig &channel);
}   // namespace poker::databus_impl