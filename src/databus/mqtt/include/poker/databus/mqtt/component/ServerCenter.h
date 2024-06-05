//
// Created by zyk on 2024-6-4 on 22-7-14.
//

#pragma once

#include <mqtt/client.h>
#include <nox/thread.h>
#include <nox/logger.h>

#include <poker/base/Record.h>
#include <poker/databus.h>

#include "../entity/config.h"
#include "../interface/ICore.h"


namespace poker::databus::mqtt
{
    class ServerCenter :
        public nox::thread::MessageLoop<::mqtt::const_message_ptr>
    {
    public:
        /**
         * @brief 服务回调器
         */
        using Handler = std::function<nox::expected<std::string>(const std::string &)>;

    public:
        /**
         * @brief 使用前准备
         */
        void Prepare(ICore * core_ptr);

        /**
         * @brief 注册指定服务通道，不允许注册两个同名的服务
         */
        AuxDeleter Serve(const std::string & service_topic, Handler handler);

        /**
         * @brief 卸载指定服务通道
         */
        void UnServe(const std::string & service_topic);

    protected:
        /**
         * @brief 消息队列初始化，设置 1/4 buffer 大小的并发规模
         */
        void OnSetup(nox::thread::MessageLoopConfig<::mqtt::const_message_ptr> & config) override;

        /**
         * @brief 将请求消息分发到对应的 server 中进行处理
         */
        void OnExecute(MessageBuffer<::mqtt::const_message_ptr> & buffer) override;

    private:
        /**
         * @brief 使用指定的服务器，处理请求消息，并答复调用者。
         */
        void Serving(const Handler & handler, const ::mqtt::const_message_ptr & request_msg);

    private:
        nox_static_logging_declaration("mqtt::ServerCenter");

    private:
        // 核心模块接口
        ICore * _core_ptr = nullptr;

        // 所有服务通道
        poker::Record<std::string, Handler> _servers;
    };
}
