//
// Created by zyk on 2024-6-4 on 22-7-14.
//

#pragma once

#include <mqtt/client.h>
#include <nox/thread.h>

#include <poker/base/Record.h>

#include "../entity/config.h"
#include "../entity/CallChannel.h"


namespace poker::databus::mqtt
{
    class CallerCenter :
        public nox::thread::MessageLoop<::mqtt::const_message_ptr>
    {
    public:
        struct Config
        {
            // 同一个调用数据通道下，同时向发发起的最大请求数量
            std::size_t max_calls_per_channel = 8;

            // 调用的超时时间
            nox::unit::Time timeout = noxu::sec(5.0);
        };

    public:
        /**
         * @brief 使用前准备
         */
        void Prepare(ICore * core_ptr, Config config);

        /**
         * @brief 发起请求，等待响应，返回响应结果。
         */
        nox::expected<std::string> Call(const std::string & request_topic, std::string request_data);

    protected:
        /**
         * @brief 消息队列初始化，设置 1/4 buffer 大小的并发规模
         */
        void OnSetup(nox::thread::MessageLoopConfig<::mqtt::const_message_ptr> & config) override;

        /**
         * @brief 消息的处理，将分析该消息属于哪一个 caller，并唤醒它
         */
        void OnExecute(MessageBuffer<::mqtt::const_message_ptr> & buffer) override;

    private:
        // 核心模块接口
        ICore * _core_ptr = nullptr;

        // 配置参数
        Config _config;

        // 所有的调用通道
        poker::Record<std::string, CallChannel> _call_channels;
    };
}
