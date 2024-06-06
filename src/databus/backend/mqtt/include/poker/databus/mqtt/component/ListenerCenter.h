//
// Created by zyk on 2024-6-4 on 22-7-14.
//

#pragma once


#include <mqtt/client.h>
#include <nox/thread.h>

#include <poker/base/Record.h>
#include <poker/databus.h>

#include "../entity/config.h"
#include "../interface/ICore.h"


namespace poker::databus::mqtt
{
    class ListenerCenter :
        public nox::thread::MessageLoop<::mqtt::const_message_ptr>
    {
    public:
        /**
         * @brief 使用前准备
         */
        void Prepare(ICore * core_ptr);

        /**
         * @brief 挂载一个新的监听函数；若该监听通道还没有初始化，则初始化他。
         * @param decoder 将用于初始化监听通道。
         * @return 监听卸载器
         */
        template<class T>
        AuxDeleter Listen(
            const std::string & topic, Handler<T> handler, const Decoder<T> & decoder)
        {
            nox_assert(not topic.empty());
            nox_assert(handler != nullptr);
            nox_assert(decoder != nullptr);

            // step 1: 取出指定的监听通道，若还未存在该监听通道，则进行创建
            //         该过程是加锁进行的。
            auto [channel_ptr, is_new_channel] = _listeners.DataPtrOrNew(topic);

            if (channel_ptr == nullptr)
                nox_no_impl("memory insufficient for new listener !");

            // step 2: 若监听通道是新建的，则进行初始化，并向 mqtt 注册该 topic 的监听。
            if (is_new_channel)
            {
                channel_ptr->Prepare<T>(
                    decoder,
                    CallbackConfig
                        {
                            .enable_discard_calls = true
                        }
                );

                _core_ptr->Subscribe(topic, 0);
            }

            // step 3: 添加新回调，获得该回调的索引编号
            std::optional<std::size_t> callback_number_opt = channel_ptr->AddCallback<T>(std::move(handler));

            // step 4: 准备通信卸载器，若回调函数设置成功，则配置该回调函数的卸载流程
            AuxDeleter deleter;

            if (callback_number_opt.has_value())
            {
                deleter.deleter_caller = [this, topic, callback_number = callback_number_opt.value()]
                {
                    this->Offline(topic, callback_number);
                };
            }

            // step 4: 释放该监听通道的锁
            _listeners.ReleasePtr(topic);

            // step 5: 返回监听卸载器
            return deleter;
        }

        /**
         * @brief 卸载指定 topic 的监听，将去除所有 callback
         */
        void Offline(const std::string & topic);

        /**
         * @brief 卸载指定 topic 的指定 callback，若最后一个 topic 被去除，则删除
         * 该监听通道。
         */
        void Offline(const std::string & topic, std::size_t callback_number);

    protected:
        /**
         * @brief 消息队列初始化，设置 1/4 buffer 大小的并发规模
         */
        void OnSetup(nox::thread::MessageLoopConfig<::mqtt::const_message_ptr> & config) override;

        /**
         * @brief 将新收到的消息，分发给对应的监听通道进行回调处理
         */
        void OnExecute(MessageBuffer<::mqtt::const_message_ptr> & buffer) override;

    private:
        // 核心模块接口
        ICore * _core_ptr = nullptr;

        // 所有监听通道
        poker::Record<std::string, SerializableListenerChannel> _listeners;
    };
}
