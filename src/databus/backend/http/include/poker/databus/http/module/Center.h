//
// Created by zyk on 24-3-7.
//

#pragma once

#include <poker/param.h>

#include <poker/databus.h>

#include "../component/CallerCenter.h"
#include "../component/ServerCenter.h"


namespace poker::databus::http
{
    class Center
    {
    public:
        struct Param
        {
            ServerCenter::Param server_param;
            CallerCenter::Param caller_param;
        };

        static bool Read(Param &param);
        static bool Write(const Param &param);

    public:
        Center()
        {
            _server_center = std::make_shared< ServerCenter >();
            _caller_center = std::make_shared< CallerCenter >();
        }

        void Setup();

        void Shutdown();

    public:
        template < class TRequest, class TResponse >
        AuxDeleter Serve(const ChannelConfigHttp &channel, Server< TRequest, TResponse > server)
        {
            _server_center->Serve< TRequest, TResponse >(channel, std::move(server));

            return AuxDeleter {.deleter_caller = [ this, &channel ]() { Offline(channel); }};
        }

        template < class TRequest, class TResponse >
        std::optional< TResponse > Call(const ChannelConfigHttp &channel, const TRequest &req)
        {
            return _caller_center->Call< TRequest, TResponse >(channel, req);
        }

        void Offline(const ChannelConfigHttp &channel)
        {
            return _server_center->Offline(channel);
        }

    private:
        Param _param;

        std::shared_ptr< ServerCenter > _server_center;
        std::shared_ptr< CallerCenter > _caller_center;
    };
}   // namespace poker::databus::http


// clang-format off
POKER_REFLECT_TYPE(
    poker::databus::http::Center::Param,
    server_param,
    caller_param
)
// clang-format on