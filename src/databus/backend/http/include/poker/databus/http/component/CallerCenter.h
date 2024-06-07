//
// Created by zyk on 24-3-7.
//

#pragma once

#include <chrono>

#include <poker/param.h>

#include <poker/databus.h>
#include <poker/serialization.h>

#include "../interface/ParamRequest.h"
#include "./detials/httplib_safe.h"
#include "./detials/thread_pool.h"


namespace poker::databus::http
{
    class CallerCenter
    {
    public:
        using RequestTask = std::packaged_task< ::httplib::Result() >;

        struct Param
        {
            // 调用允许超时时间
            poker::unit::Time wait_for_time_out = pokeru::ms(3'000);

            // 同时处理请求的最大线程数
            std::size_t max_thread_nums = 8;
        };

    public:
        void SetParam(const Param &param)
        {
            _param = param;
        }

        void Setup()
        {
            // 初始化线程池
            _thread_pool.Init(_param.max_thread_nums);
        }

        void Shutdown()
        {
            _thread_pool.ShutDown();
        }

        template < class TRequest, class TResponse >
        std::optional< TResponse > Call(const ChannelConfigHttp &channel, const TRequest &req)
        {
            switch (channel.method)
            {
#define DISPATCH_CALL(_TYPE_) \
    case Method::_TYPE_:      \
        return InnerCall< TRequest, TResponse >(channel, req, _TYPE_());

                POKER_DATABUS_ALL_HTTP_METHOD(DISPATCH_CALL)

#undef DISPATCH_CALL

                default:
                    poker_no_impl("undefined http method");
            }
        }

    protected:
        /**
         * @brief 未指定 method 时的默认处理
         */
        template < class TRequest, class TResponse >
        std::optional< TResponse > InnerCall(const ChannelConfigHttp &channel, const TRequest &req, Method method_tag)
        {
            return InnerCall< TRequest, TResponse >(channel, req, Post());
        }

        template < class TRequest, class TResponse >
        std::optional< TResponse > InnerCall(const ChannelConfigHttp &channel, const TRequest &req, Get method_tag)
        {
            // 创建任务
            RequestTask get_task(
                    [ &channel, &req ]() -> ::httplib::Result
                    {
                        // 创建客户端
                        ::httplib::Client client(channel.ip, channel.port);

                        // 请求服务

                        // 携带 param
                        if constexpr (std::is_base_of_v< IParamRequest, TRequest >)
                        {
                            // 取出参数
                            ::httplib::Params params;

                            req.Visit([ &params ](std::string name, std::string value)
                                      { params.emplace(std::move(name), std::move(value)); });

                            ::httplib::Result result = client.Get(channel.request_target, params, ::httplib::Headers());

                            return result;
                        }
                        // 忽略不支持的请求参数
                        else
                        {
                            ::httplib::Result result = client.Get(channel.request_target);

                            return result;
                        }
                    });

            // 执行任务并返回请求结果
            return ProcessCall< TResponse >(std::move(get_task));
        }

        template < class TRequest, class TResponse >
        std::optional< TResponse > InnerCall(const ChannelConfigHttp &channel, const TRequest &req, Post method_tag)
        {
            // 创建任务
            RequestTask get_task(
                    [ &channel, &req ]() -> ::httplib::Result
                    {
                        // 创建客户端
                        ::httplib::Client client(channel.ip, channel.port);

                        // 序列化请求数据
                        std::string binary_req = serialization::Encode(req);

                        // 请求服务
                        ::httplib::Result result = client.Post(channel.request_target, binary_req, "text/plain");

                        return result;
                    });

            // 执行任务并返回请求结果
            return ProcessCall< TResponse >(std::move(get_task));
        }

        template < class TRequest, class TResponse >
        std::optional< TResponse > InnerCall(const ChannelConfigHttp &channel, const TRequest &req, Patch method_tag)
        {
            // 创建任务
            RequestTask get_task(
                    [ &channel, &req ]() -> ::httplib::Result
                    {
                        // 创建客户端
                        ::httplib::Client client(channel.ip, channel.port);

                        // 序列化请求数据
                        std::string binary_req = serialization::Encode(req);

                        // 请求服务
                        ::httplib::Result result = client.Patch(channel.request_target, binary_req, "text/plain");

                        return result;
                    });

            // 执行任务并返回请求结果
            return ProcessCall< TResponse >(std::move(get_task));
        }

        template < class TRequest, class TResponse >
        std::optional< TResponse > InnerCall(const ChannelConfigHttp &channel, const TRequest &, Delete method_tag)
        {
            // 创建任务
            RequestTask get_task(
                    [ &channel ]() -> ::httplib::Result
                    {
                        // 创建客户端
                        ::httplib::Client client(channel.ip, channel.port);

                        // 请求服务
                        ::httplib::Result result = client.Delete(channel.request_target);

                        return result;
                    });

            // 执行任务并返回请求结果
            return ProcessCall< TResponse >(std::move(get_task));
        }

        template < class TRequest, class TResponse >
        std::optional< TResponse > InnerCall(const ChannelConfigHttp &channel, const TRequest &req, Options method_tag)
        {
            // 创建任务
            RequestTask get_task(
                    [ &channel ]() -> ::httplib::Result
                    {
                        // 创建客户端
                        ::httplib::Client client(channel.ip, channel.port);

                        // 请求服务
                        ::httplib::Result result = client.Options(channel.request_target);

                        return result;
                    });

            // 执行任务并返回请求结果
            return ProcessCall< TResponse >(std::move(get_task));
        }

    private:
        template < class TResponse >
        std::optional< TResponse > ProcessCall(RequestTask &&task)
        {
            // 计算超时点
            auto deadline_time_point =
                    std::chrono::steady_clock::now() +
                    std::chrono::milliseconds(std::size_t(_param.wait_for_time_out.Get< pokeru::ms >()));

            std::future< ::httplib::Result > get_result = task.get_future();

            // 执行任务
            _thread_pool.AddRequest(std::move(task));

            // 等待任务执行结果
            if (get_result.wait_until(deadline_time_point) == std::future_status::ready)
            {
                ::httplib::Result result = get_result.get();

                // 请求成功
                if (result and result->status == 200)
                {
                    // 尝试解析结果
                    std::optional< TResponse > rep = serialization::Decode< TResponse >(result->body);

                    return rep;
                }
                else
                {
                    return {};
                }
            }
            else
            {
                return {};
            }
        }

    private:
        Param _param;

        // 线程池
        details::ThreadPool _thread_pool;
    };
}   // namespace poker::databus::http


// clang-format off
POKER_REFLECT_TYPE(
    poker::databus::http::CallerCenter::Param,
    wait_for_time_out,
    max_thread_nums
)
// clang-format on