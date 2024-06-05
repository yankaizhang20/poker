//
// Created by zyk on 2024-6-5
//

#pragma once

#include <poker/databus.h>
#include <poker/macro.h>
#include <poker/pattern.h>

#include "./this_process/module/Center.h"


// 进程内的通信接口的实现
namespace poker::databus_impl::this_process
{
    inline decltype(auto) Center()
    {
        return pattern::GetGlobalUnique< databus::this_process::Center >::Ref();
    }

    template < class T >
    void Send(const std::string &channel, const T &data)
    {
        Center().Send(channel, data);
    }

    template < class T >
    AuxDeleter Listen(const std::string &channel, const databus::Handler< T > &handler)
    {
        return Center().Listen(channel, handler);
    }

    template < class Request, class Response >
    std::optional< Response > Call(const std::string &channel, const Request &req)
    {
        return Center().Call< Request, Response >(channel, req);
    }

    template < class Request, class Response >
    AuxDeleter Serve(const std::string &channel, const databus::Server< Request, Response > &server)
    {
        return Center().Serve(channel, server);
    }

    inline void TopicOffline(const std::string &channel)
    {
        Center().TopicOffline(channel);
    }

    inline void ServiceOffline(const std::string &channel)
    {
        Center().ServiceOffline(channel);
    }
}   // namespace poker::databus_impl::this_process
