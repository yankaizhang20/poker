//
// Created by zyk on 2024-6-5
//

#pragma once

#include <poker/databus.h>

#include "../component/AnyTypeListenerChannel.h"
#include "../component/ServiceWrapper.h"
#include "../component/details/CallbackConfig.h"


namespace poker::databus::this_process
{
    /**
     * @brief 进程内多线程间的数据交换中心，支持发送、监听、服务等形式，实现异步消息分发。
     */
    class Center
    {
    public:
        template < class T >
        void Send(const std::string &topic, const T &data)
        {
            ReaderLocking(_listeners_keeper)
            {
                if (_listeners.find(topic) == _listeners.end())
                {
                    return;
                }
            }

            WriterLocking(_listeners_keeper)
            {
                // 调用这个通道的全部回调器
                _listeners[ topic ].Notify(data);
            }
        }

        template < class T >
        AuxDeleter Listen(const std::string &topic, Handler< T > handler)
        {
            // step 0: 不允许空监听
            if (topic.empty() or handler == nullptr)
                return {};

            // 回调索引编号
            std::optional< std::size_t > callback_number_opt;

            // step 1: 取出指定的监听通道，若还未存在该监听通道，则进行创建
            //         该过程是加锁进行的。
            WriterLocking(_listeners_keeper)
            {
                // step 2: 若监听通道是新建的，则进行初始化
                if (_listeners.find(topic) == _listeners.end())
                {
                    _listeners[ topic ].Prepare< T >(details::CallbackConfig {.enable_discard_calls = false});
                }

                // step 3: 添加新回调，获得该回调的索引编号
                callback_number_opt = _listeners[ topic ].AddCallback< T >(std::move(handler));
            }

            // step 4: 准备通信卸载器，若回调函数设置成功，则配置该回调函数的卸载流程
            AuxDeleter deleter;

            if (callback_number_opt.has_value())
            {
                deleter.deleter_caller = [ this, topic, callback_number = callback_number_opt.value() ]
                { this->RemoveCallback(topic, callback_number); };
            }

            // step 5: 返回监听卸载器
            return deleter;
        }

        template < class TRequest, class TResponse >
        std::optional< TResponse > Call(const std::string &topic, const TRequest &request)
        {
            ReaderLocking(_servers_keeper)
            {
                // 获取失败，说明该通道还未挂载服务器，直接返回错误
                if (_servers.find(topic) == _servers.end())
                {
                    return {};
                }
            }

            // 创建响应数据，执行服务调用
            TResponse response;

            bool status = false;

            WriterLocking(_servers_keeper)
            {
                status = _servers[ topic ].CallService< TRequest, TResponse >(request, response);
            }

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
            WriterLocking(_servers_keeper)
            {
                // 设置服务器
                _servers[ topic ].SetService< TRequest, TResponse >(server);
            }

            // 设置服务器卸载器
            deleter.deleter_caller = [ this, topic ]() { this->RemoveServer(topic); };

            return deleter;
        }

        void TopicOffline(const std::string &channel);

        void ServiceOffline(const std::string &channel);

    private:
        void RemoveCallback(const std::string &topic, std::size_t callback_number);

        void RemoveServer(const std::string &topic);

    private:
        // 由不同通道名索引的回调容器，每个通道名仅允许一种类型
        thread::SharedMutexLock                         _listeners_keeper;
        std::map< std::string, AnyTypeListenerChannel > _listeners;

        // 由不同通道名索引的服务器，每个通道名仅允许一种类型
        thread::SharedMutexLock                 _servers_keeper;
        std::map< std::string, ServiceWrapper > _servers;
    };
}   // namespace poker::databus::this_process
