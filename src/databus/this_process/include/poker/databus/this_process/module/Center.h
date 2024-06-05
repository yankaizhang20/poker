//
// Created by zyk on 2024-6-5
//

#pragma once

#include <poker/databus.h>

#include "../component/ServiceWrapper.h"


namespace poker::databus::this_process
{
    /**
     * @brief 进程内多线程间的数据交换中心，支持发送、监听、服务等形式，实现异步消息分发。
     */
    class Center
    {
    public:
        Center();

        template < class T >
        void Send(const std::string &topic, const T &data)
        {
            // 取出回调管理器，并对其上锁
            auto *ptr = _listeners.DataPtr(topic);

            if (ptr == nullptr)
                return;

            // 调用这个通道的全部回调器
            ptr->Notify(data);

            // 释放数据锁
            _listeners.ReleasePtr(topic);
        }

        template < class T >
        AuxDeleter Listen(const std::string &topic, Handler< T > handler)
        {
            // step 0: 不允许空监听
            if (topic.empty() or handler == nullptr)
                return {};

            // step 1: 取出指定的监听通道，若还未存在该监听通道，则进行创建
            //         该过程是加锁进行的。
            auto [ channel_ptr, is_new_channel ] = _listeners.DataPtrOrNew(topic);

            if (channel_ptr == nullptr)
                poker_no_impl("memory insufficient for new listener !");

            // step 2: 若监听通道是新建的，则进行初始化
            if (is_new_channel)
            {
                channel_ptr->Prepare< T >(CallbackConfig {.enable_discard_calls = false});
            }

            // step 3: 添加新回调，获得该回调的索引编号
            std::optional< std::size_t > callback_number_opt = channel_ptr->AddCallback< T >(std::move(handler));

            // step 4: 准备通信卸载器，若回调函数设置成功，则配置该回调函数的卸载流程
            AuxDeleter deleter;

            if (callback_number_opt.has_value())
            {
                deleter.deleter_caller = [ this, topic, callback_number = callback_number_opt.value() ]
                { this->RemoveCallback(topic, callback_number); };
            }

            // step 4: 释放该监听通道的锁
            _listeners.ReleasePtr(topic);

            // step 5: 返回监听卸载器
            return deleter;
        }

        template < class TRequest, class TResponse >
        std::optional< TResponse > Call(const std::string &topic, const TRequest &request)
        {
            // 取出服务器管理器，并对其上锁
            ServiceWrapper *ptr = _servers.DataPtr(topic);

            // 获取失败，说明该通道还未挂载服务器，直接返回错误
            if (ptr == nullptr)
                return {};

            // 创建响应数据，执行服务调用
            TResponse response;

            bool status = ptr->CallService< TRequest, TResponse >(request, response);

            // 释放数据锁
            _servers.ReleasePtr(topic);

            // 返回结果
            if (status)
                return response;
            else
                return {};
        }

        template < class TRequest, class TResponse >
        AuxDeleter Serve(const std::string &topic, const Server< TRequest, TResponse > &server)
        {
            // 将用于卸载新挂载的服务器
            AuxDeleter deleter;

            // 若用户设置了空指针，则直接返回
            if (server == nullptr)
                return deleter;

            // 取出服务器管理器，并对其上锁
            auto [ ptr, _ ] = _servers.DataPtrOrNew(topic);

            if (ptr == nullptr)
            {
                std::cout << "no enough space for new server !" << std::endl;
                return deleter;
            }

            // 设置服务器
            ptr->SetService< TRequest, TResponse >(server);

            // 设置服务器卸载器
            deleter.deleter_caller = [ this, topic ]() { this->RemoveServer(topic); };

            // 释放数据锁
            _servers.ReleasePtr(topic);

            return deleter;
        }

        void TopicOffline(const std::string &channel);

        void ServiceOffline(const std::string &channel);

    private:
        void RemoveCallback(const std::string &topic, std::size_t callback_number);

        void RemoveServer(const std::string &topic);

    private:
        // 由不同通道名索引的回调容器，每个通道名仅允许一种类型
        poker::Record< std::string, AnyTypeListenerChannel > _listeners;

        // 由不同通道名索引的服务器，每个通道名仅允许一种类型
        poker::Record< std::string, ServiceWrapper > _servers;
    };
}   // namespace poker::databus::this_process
