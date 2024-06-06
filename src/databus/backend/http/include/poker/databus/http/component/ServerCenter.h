//
// Created by zyk on 24-3-7.
//

#pragma once

#include <map>
#include <mutex>
#include <string>
#include <thread>

#include <poker/databus.h>
#include <poker/macro.h>
#include <poker/param.h>
#include <poker/serialization.h>

#include "../entity/Method.h"
#include "./detials/httplib_safe.h"


namespace poker::databus::http
{
    class ServerCenter final
    {
    public:
        // 参数配置
        struct Param
        {
            std::string server_address = "localhost";
            std::size_t port           = 65'534;
        };

    public:
        ServerCenter()
        {
            _server = std::make_shared< ::httplib::Server >();
        }

        void SetParam(const Param &param)
        {
            _param = param;
        }

        void Setup()
        {
            // TODO: 确认 ip port 是否有效

            // 启动监听线程
            std::thread listenner(
                    [ this ]()
                    {
                        std::cout << "http server listening " << _param.server_address << ":" << _param.port
                                  << std::endl;

                        if (!_server->listen(_param.server_address, (int) _param.port))
                        {
                            std::cerr << "http server failed to listen " << _param.server_address << ":" << _param.port
                                      << std::endl;
                        }
                    });

            listenner.detach();
        }

        void Shutdown()
        {
        }

        template < class TMethod, class TRequest, class TResponse >
        void Serve(const XChannelType &channel, std::function< bool(const TRequest &, TResponse &) > server)
        {
            InnerServe< TRequest, TResponse >(channel, std::move(server), TMethod {});
        }

        template < class TMethod >
        void Offline(const XChannelType &channel)
        {
            poker_no_impl("不支持的服务卸载");
        }

    protected:
        template < class TRequest, class TResponse >
        void InnerServe(const XChannelType &channel, std::function< bool(const TRequest &, TResponse &) > server,
                        const Method &method_tag)
        {
            InnerServe< TRequest, TResponse >(channel, server, Post());
        }

        template < class TRequest, class TResponse >
        void InnerServe(const XChannelType &channel, std::function< bool(const TRequest &, TResponse &) > server,
                        const Get &method_tag)
        {
            _server->Get(channel.request_target,
                         [ server = std::move(server), this ](const ::httplib::Request &req, ::httplib::Response &rep)
                         { ProcessServe(server, req, rep); });
        }

        template < class TRequest, class TResponse >
        void InnerServe(const XChannelType &channel, std::function< bool(const TRequest &, TResponse &) > server,
                        const Post &method_tag)
        {
            _server->Post(channel.request_target,
                          [ server = std::move(server), this ](const ::httplib::Request &req, ::httplib::Response &rep)
                          { ProcessServe(server, req, rep); });
        }

        template < class TRequest, class TResponse >
        void InnerServe(const XChannelType &channel, std::function< bool(const TRequest &, TResponse &) > server,
                        const Patch &method_tag)
        {
            _server->Patch(channel.request_target,
                           [ server = std::move(server), this ](const ::httplib::Request &req, ::httplib::Response &rep)
                           { ProcessServe(server, req, rep); });
        }

        template < class TRequest, class TResponse >
        void InnerServe(const XChannelType &channel, std::function< bool(const TRequest &, TResponse &) > server,
                        const Delete &method_tag)
        {
            _server->Delete(channel.request_target, [ server = std::move(server), this ](const ::httplib::Request &req,
                                                                                         ::httplib::Response      &rep)
                            { ProcessServe(server, req, rep); });
        }

        template < class TRequest, class TResponse >
        void InnerServe(const XChannelType &channel, std::function< bool(const TRequest &, TResponse &) > server,
                        const Options &method_tag)
        {
            _server->Options(channel.request_target, [ server = std::move(server), this ](const ::httplib::Request &req,
                                                                                          ::httplib::Response      &rep)
                             { ProcessServe(server, req, rep); });
        }

    protected:
        template < class TRequest, class TResponse >
        bool ProcessServe(const std::function< bool(const TRequest &, TResponse &) > &server,
                          const ::httplib::Request &req, ::httplib::Response &rep) const
        {
            // 解析请求
            std::optional< TRequest > inner_req = serialization::Decode< TRequest >(req.body);

            if (!inner_req.has_value())
            {
                poker_no_impl("未定义的序列化方式");
            }

            // 调用服务
            TResponse inner_rep;
            if (server(inner_req.value(), inner_rep))
            {
                // 设置响应内容
                rep.body = serialization::Encode(inner_rep);

                return true;
            }
            else
            {
                return false;
            }
        }

    private:
        Param _param;

        std::shared_ptr< ::httplib::Server > _server;
    };

}   // namespace poker::databus::http


// clang-format off
POKER_REFLECT_TYPE(
        poker::databus::http::ServerCenter::Param,
        server_address,
        port
)
// clang-format on