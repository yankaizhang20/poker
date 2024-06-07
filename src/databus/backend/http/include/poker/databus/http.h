//
// Created by zyk on 24-3-6.
//
//
#pragma once

#include <poker/databus.h>

#include "./http/interface/ParamRequest.h"
#include "./http/module/Center.h"
#include "./http/module/CenterModule.h"


namespace poker::databus_impl::http
{
    inline decltype(auto) Center()
    {
        return poker::pattern::GetGlobalUnique< databus::http::Center >::Ref();
    };

    /**
     * @brief 调用指定 http 通道中的服务过程
     */
    template < class TRequest, class TResponse >
    std::optional< TResponse > Call(const databus::ChannelConfigHttp &channel, const TRequest &req)
    {
        return Center().Call< TRequest, TResponse >(channel, req);
    }

    /**
     * @brief 在指定 http 通道中，注册一个服务过程。
     */
    template < class TRequest, class TResponse >
    AuxDeleter Serve(const databus::ChannelConfigHttp &channel, Server< TRequest, TResponse > server)
    {
        return Center().Serve< TRequest, TResponse >(channel, std::move(server));
    }

    /**
     * @brief 关闭指定 http 通道
     */
    void ServiceOffline(const databus::ChannelConfigHttp &channel)
    {
        Center().Offline(channel);
    }
}   // namespace poker::databus_impl::http