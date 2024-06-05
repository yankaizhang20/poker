//
// Created by zyk on 2024-6-4 on 22-7-15.
//

#pragma once

#include <mqtt/client.h>


namespace poker::databus::mqtt
{
    class ICore
    {
    public:
        /**
         * @return 统一的响应数据 topic 的前缀，用于区分一般数据输入。
         */
        virtual const std::string & GetResponseTopicPrefix() const = 0;

        /**
         * @brief 监听指定消息，同时将其加入断开重连列表中。
         */
        virtual void Subscribe(const std::string & topic, int qos) = 0;

        /**
         * @brief 取消指定消息的监听，并将其从断开重连列表中删除。
         */
        virtual void Unsubscribe(const std::string & topic) = 0;

        /**
         * @brief 发布一个 mqtt 消息
         */
        virtual void Publish(::mqtt::message_ptr msg) = 0;
    };
}
