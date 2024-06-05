//
// Created by zyk on 2024-6-4 on 22-7-14.
//

#pragma once

#include <poker/param.h>
#include <poker/serialization.h>
#include <poker/thread/Loop.h>

#include "../interface/ICore.h"

#include "../component/CallerCenter.h"
#include "../component/ListenerCenter.h"
#include "../component/ServerCenter.h"


namespace poker::databus::mqtt
{
    class Center : private ICore, private thread::Loop
    {
    public:
        struct Param
        {
            // 服务器地址
            std::string server_address;

            // 响应数据 topic 名称前缀
            std::string response_topic_prefix = "poker/mqtt_reply/";

            // 同一个调用数据通道下，同时向发发起的最大请求数量
            std::size_t max_calls_per_channel = 8;

            // mqtt 服务线程并发数量
            std::size_t max_parallel_serving_threads = 4;

            // 远程调用的超时时间
            poker::unit::Time call_timeout = pokeru::sec(5.0);

            // 重连超时重置的时间，若 mqtt 重连超过该时间阈值时，尝试重建 mqtt 对象
            poker::unit::Time reconnect_timeout = pokeru::sec(60.0);
        };

        static bool Read(Param &param);

        static bool Write(const Param &param);

    public:
        /**
         * @brief 初始化 mqtt 客户端，启动所有分发流程
         */
        void Setup();

        /**
         * @brief 关闭 mqtt 客户端，关闭所有的分发流程。
         */
        void Shutdown();

    public:
        template < class T >
        void Send(const std::string &topic, const T &data)
        {
            // step 1: 创建发送消息
            ::mqtt::message_ptr msg = ::mqtt::message::create(topic, serialization::Encode(data), 1, false);

            // step 2: 获取 mqtt 客户端
            ::mqtt::async_client_ptr client_ptr = GetMqttClientPtr();

            // mqtt 未初始化时，不进行处理
            if (client_ptr == nullptr)
                return;

            // step 3: 发送该消息
            try
            {
                client_ptr->publish(std::move(msg));
            }
            catch (const std::exception &ec)
            {
                std::cout << "publish fail: " << ec.what();
            }
        }

        template < class T >
        AuxDeleter Listen(const std::string &topic, Handler< T > handler)
        {
            // 本类型的反序列化器
            static auto decoder = [](const std::string &data) -> std::optional< T >
            { return serialization::Decode< T >(data); };

            // 若传入的回调为空，则不发生任何事情
            if (handler == nullptr)
                return {};

            // 向监听者中心注册新的回调
            return _listener_center.Listen< T >(topic, std::move(handler), decoder);
        }

        template < class TRequest, class TResponse >
        std::optional< TResponse > Call(const std::string &request_topic, const TRequest &request)
        {
            // 序列化请求数据，并发起调用
            std::optional< std::string > response_str_opt =
                    _caller_center.Call(request_topic, serialization::Encode(request));

            // 若得到的响应数据非空，则尝试反序列化它，返回结果。
            if (response_str_opt.has_value())
            {
                std::optional< TResponse > response_opt = serialization::Decode< TResponse >(response_str_opt.value());

                if (response_opt.has_value())
                    return response_opt;
            }

            // 若响应数据非法，则返回错误
            return {};
        }

        template < class TRequest, class TResponse >
        AuxDeleter Serve(const std::string &service_topic, Server< TRequest, TResponse > server)
        {
            // 若传入的回调为空，则不发生任何事情
            if (server == nullptr)
                return {};

            // 向服务者中心注册新的服务函数
            return _server_center.Serve(
                    service_topic,
                    [ server = server ](const std::string &request_str) -> std::optional< std::string >
                    {
                        // 反序列化请求数据
                        std::optional< TRequest > request_opt = serialization::Decode< TRequest >(request_str);

                        // 若反序列化成功，且服务函数返回正确结果，则序列化响应数据，并返回；
                        // 否则返回错误。
                        if (TResponse response; request_opt.has_value() and server(request_opt.value(), response))
                            return serialization::Encode(response);
                        else
                            return {};
                    });
        }

        void TopicOffline(const std::string &topic);

        void ServiceOffline(const std::string &service);

    private:
        /**
         * @return 统一的响应数据 topic 的前缀，用于区分一般数据输入。
         */
        const std::string &GetResponseTopicPrefix() const override;

        /**
         * @brief 监听指定消息，同时将其加入断开重连列表中。
         */
        void Subscribe(const std::string &topic, int qos) override;

        /**
         * @brief 取消指定消息的监听，并将其从断开重连列表中删除。
         */
        void Unsubscribe(const std::string &topic) override;

        /**
         * @brief 发布一个 mqtt 消息
         */
        void Publish(::mqtt::message_ptr msg) override;

    private:
        /**
         * @brief mqtt 消息处理主循环
         */
        void OnRun(Loop *self) override;

        /**
         * @brief 重建 mqtt client 对象，并执行连接过程
         */
        void ResetMqttAndConnect();

        /**
         * @brief 发起与 broker 的连接，返回是否连接成功。
         */
        bool Connect();

        /**
         * @brief 发起与 broken 的重连接，返回是否连接成功。
         */
        bool Reconnect();

    private:
        /**
         * @brief 获取 mqtt 客户端指针，可能返回空指针
         * @note 本函数用于异步的访问流程，本类的主流程，无需使用该函数
         */
        ::mqtt::async_client_ptr GetMqttClientPtr() const;

    private:
        struct Connection
        {
            int qos = 0;
        };

    private:
        // mqtt 核心客户端
        MutexLockable(::mqtt::async_client_ptr) _core_ptr;

        // 模块参数，模块启动时初始化
        Param _param;

        // mqtt 响应数据固定前缀
        std::string _response_topic_prefix;

    private:
        // 监听者中心，用于消化普通数据的输入
        ListenerCenter _listener_center;

        // 调用者管理中心，用于消化响应数据的输入
        CallerCenter _caller_center;

        // 服务器管理中心，用于消化请求数据的输入
        ServerCenter _server_center;

        // 断开重连列表
        MutexLockable(std::map< std::string, Connection >) _reconnection_list;
    };
}   // namespace poker::databus::mqtt


// clang-format off
POKER_REFLECT_TYPE(poker::databus::mqtt::Center::Param,
                    server_address,
                    response_topic_prefix,
                    max_calls_per_channel,
                    max_parallel_serving_threads,
                    call_timeout,
                    reconnect_timeout
)
// clang-format on
