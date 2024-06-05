//
// Created by zyk on 2024-6-4 on 22-7-14.
//

#include <poker/pattern.h>

#include "./mqtt/module/Center.h"
#include "./mqtt/module/CenterModule.h"


namespace poker::databus_impl::mqtt
{
    inline decltype(auto) Center()
    {
        return pattern::GetGlobalUnique< databus::mqtt::Center >::Ref();
    }

    template < class T >
    void Send(const std::string &channel, const T &data)
    {
        Center().Send(channel, data);
    }

    template < class T >
    AuxDeleter Listen(const std::string &channel, databus::Handler< T > handler)
    {
        return Center().Listen(channel, std::move(handler));
    }

    template < class Request, class Response >
    std::optional< Response > Call(const std::string &channel, const Request &request)
    {
        return Center().Call< Request, Response >(channel, request);
    }

    template < class Request, class Response >
    AuxDeleter Serve(const std::string &channel, databus::Server< Request, Response > server)
    {
        return Center().Serve(channel, std::move(server));
    }

    inline void TopicOffline(const std::string &channel)
    {
        Center().TopicOffline(channel);
    }

    inline void ServiceOffline(const std::string &channel)
    {
        Center().ServiceOffline(channel);
    }
}   // namespace poker::databus_impl::mqtt
