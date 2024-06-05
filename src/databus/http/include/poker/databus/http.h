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
    template < class TMethod, class TRequest, class TResponse >
    std::optional< TResponse > Call(const databus::XChannelType &channel, const TRequest &req)
    {
        return Center().Call< TMethod, TRequest, TResponse >(channel, req);
    }

    /**
     * @brief 在指定 http 通道中，注册一个服务过程。
     */
    template < class TMethod, class TRequest, class TResponse >
    AuxDeleter Serve(const databus::XChannelType &channel, Server< TRequest, TResponse > server)
    {
        return Center().Serve< TMethod, TRequest, TResponse >(channel, std::move(server));
    }

    /**
     * @brief 关闭指定 http 通道
     */
    template < class TMethod >
    void Offline(const databus::XChannelType &channel)
    {
        Center().Offline< TMethod >(channel);
    }
}   // namespace poker::databus_impl::http