//
// Created by zyk on 24-6-4.
//

#pragma once

#include <optional>

#include "./entity/AuxDeleter.h"
#include "./entity/XChannelType.h"
#include "./entity/typedef.h"


namespace poker::databus_impl
{
    using AuxDeleter = databus::AuxDeleter;

    template < class Message >
    using Handler = databus::Handler< Message >;

    template < class Request, class Response >
    using Server = databus::Server< Request, Response >;

    /**
     * @brief 通道字符串名称
     * @tparam Channel 通道类型标签
     */
    template < class Channel >
    std::string channel_name(typeid(Channel).name());

    /**
     * @brief 通过指定字符串名称的通道，发送指定类型的数据
     */
    template < class T, class Tag = void >
    extern void Send(const std::string &channel, const T &data);

    /**
     * @brief 订阅指定字符串名称的通道，监听接收指定类型的数据
     */
    template < class T, class Tag = void >
    extern AuxDeleter Listen(const std::string &channel, const Handler< T > &handler);

    /**
     * @brief 调用指定通道中的服务过程，发出请求数据，并得到响应数据。
     */
    template < class Request, class Response, class Tag = void >
    extern std::optional< Response > Call(const std::string &channel, const Request &req);

    /**
     * @brief 在指定通道中，注册一个服务过程。
     */
    template < class Request, class Response, class Tag = void >
    extern AuxDeleter Serve(const std::string &channel, const Server< Request, Response > &server);

    /**
     * @brief 关闭指定字符串名称的通道；依然携带类型标签，是为了实现不同标签可以有不同的
     */
    template < class Channel >
    extern void Offline(const std::string &channel);
}   // namespace poker::databus_impl


// http channel 的实现接口，需要在应用层特化其实现
namespace poker::databus_impl
{
    // clang-format off
    /**
     * @brief 通道配置接口
     * @tparam TChannel 通道类型标签
     */
    template < class TChannel>
    databus::XChannelType channel_config = {
            .ip = "localhost",
            .port = 65534,
            .request_target = typeid(TChannel).name()
    };
    // clang-format on

    /**
     * @brief 通道方法接口，默认使用 Post 方法
     * @tparam TChannel 通道类型标签
     */
    template < class TChannel >
    struct channel_method
    {
        using type = databus::Method;
    };

    /**
     * @brief 调用指定通道中的服务过程，发出请求数据，并得到响应数据。
     */
    template < class TRequest, class TResponse, class Tag = void >
    extern std::optional< TResponse > Call(const databus::XChannelType &channel, const TRequest &req);

    /**
     * @brief 在指定通道中，注册一个服务过程。
     */
    template < class TRequest, class TResponse, class Tag = void >
    extern AuxDeleter Serve(const databus::XChannelType &channel, const Server< TRequest, TResponse > &server);

    /**
     * @brief 关闭指定 http 通道
     */
    template < class TChannel >
    extern void Offline(const databus::XChannelType &channel);
}   // namespace poker::databus_impl


namespace poker::databus::details
{
    /**
     * @brief 获取 channel 具体值的接口，可特化实现为不同的类型
     * @tparam ChannelType 改变 channel 类型的默认值，将影响所有的 databus 默认实现。
     */
    template < class ChannelType = std::string >
    struct ChannelImpl
    {
        template < class Channel >
        static const ChannelType &value()
        {
            return databus_impl::channel_name< Channel >;
        }
    };

    template <>
    struct ChannelImpl< XChannelType >
    {
        template < class Channel >
        static const XChannelType &value()
        {
            return databus_impl::channel_config< Channel >;
        }
    };

}   // namespace poker::databus::details