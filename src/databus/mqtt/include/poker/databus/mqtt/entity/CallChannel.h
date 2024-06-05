//
// Created by zyk on 2024-6-4 on 22-7-15.
//

#pragma once

#include <vector>
#include <mqtt/client.h>
#include <nox/thread.h>
#include <nox/chrono.h>
#include <nox/logger.h>
#include <nox/container/Timeliness.h>

#include "../interface/ICore.h"


namespace poker::databus::mqtt
{
    class CallChannel
    {
    public:
        struct Config
        {
            // 在同一个通道下，同时向外发起的最大请求数量
            std::size_t max_calls_count = 3;

            // 本通道的请求 topic 名
            std::string request_topic;

            // 超时时间
            nox::unit::Time timeout = noxu::sec(5.0);
        };

    public:
        CallChannel(ICore * core_ptr, Config config);

    public:
        /**
         * @brief 使用指定数据，发起请求，并等待回复，返回等待的结果。
         */
        nox::expected<std::string> Call(std::string request_data);

        /**
         * @brief 提交一个新响应数据
         */
        void NotifyResponse(const ::mqtt::const_message_ptr & response_msg);

    private:
        struct CallContext
        {
            // 全局唯一的、每次调用均不相同的、用于判别响应数据是否对应请求数据的
            // 数据编号。
            std::atomic<long long> correlation_number = nox::chrono::us();

            // 可等待的消息
            nox::container::Timeliness<std::string> message;

            CallContext() = default;
            CallContext(CallContext && other) noexcept;
        };

        nox_static_logging_declaration("mqtt::CallChannel");

    private:
        // 核心模块操作接口
        ICore * _core_ptr = nullptr;

        // 本模块的配置
        Config _config;

        // 要求服务者答复用的 topic
        std::string _response_topic;

    private:
        // 调用者上下文数据
        std::vector<CallContext> _call_contexts;

        // 当前可用的上下文索引
        moodycamel::ConcurrentQueue<std::size_t> _available_context_indexes;

        // 有上下文可用的信号量
        nox::thread::Semaphore _have_available_context;
    };
}
