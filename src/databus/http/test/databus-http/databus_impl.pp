//
// Created by zyk on 24-3-11.
//
#include "poker/databus/http.h"
#include "./channel.h"

namespace poker::databus_impl
{
    // 指定 MyServiceChannel 的 Method
    //    template <>
    //    struct channel_method< channel::MyServiceChannel >
    //    {
    //        using type = databus::http::Post;
    //    };

    /**
     * @brief 调用 MyServiceChannel 中的服务过程，发出请求数据，并得到响应数据。
     */
    template <>
    std::optional< int > Call< int, int, tag::MyTag >(const databus::XChannelType &channel, const int &req)
    {
        std::cout << "向服务器 " << channel.ip << ":" << channel.port << " 请求 " << channel.request_target
                  << " 资源 method=" << channel_method< channel::MyServiceChannel >::type ::name() << std::endl;

        return databus_impl::http::Call< channel_method< channel::MyServiceChannel >::type, int, int >(channel, req);
    }

    /**
     * @brief 向 MyServiceChannel 通道注册一个服务过程。
     */
    template <>
    AuxDeleter Serve< int, int, tag::MyTag >(const databus::XChannelType &channel, const Server< int, int > &server)
    {
        std::cout << "向服务器 " << channel.ip << ":" << channel.port << " 注册服务 "
                  << channel_method< channel::MyServiceChannel >::type ::name() << std::endl;

        return databus_impl::http::Serve< channel_method< channel::MyServiceChannel >::type, int, int >(channel,
                                                                                                        server);
    }

    /**
     * @brief 关闭指定 http 通道
     */
    template < class Channel >
    void Offline(const databus::XChannelType &channel)
    {
        databus_impl::http::Offline< channel_method< Channel > >(channel);
    }
}   // namespace poker::databus_impl