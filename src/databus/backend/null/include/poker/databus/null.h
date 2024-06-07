//
// Created by zyk on 2024-6-6
//

#pragma once

#include <poker/databus.h>


namespace poker::databus_impl::null
{
    template < class T >
    inline void Send(const databus::ChannelConfig &, const T &)
    {
    }

    template < class T >
    inline AuxDeleter Listen(const databus::ChannelConfig &, const databus::Handler< T > &)
    {
        return {};
    }

    template < class Request, class Response >
    inline std::optional< Response > Call(const databus::ChannelConfig &, const Request &)
    {
        return {};
    }

    template < class Request, class Response >
    inline AuxDeleter Serve(const databus::ChannelConfig &, const databus::Server< Request, Response > &)
    {
        return {};
    }

    inline void TopicOffline(const databus::ChannelConfig &)
    {
    }

    inline void ServiceOffline(const databus::ChannelConfig &)
    {
    }
}   // namespace poker::databus_impl::null
