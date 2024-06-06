//
// Created by zyk on 2024-6-5
//

#include "poker/databus/this_process/module/Center.h"


namespace poker::databus::this_process
{
    void Center::RemoveCallback(const std::string &topic, std::size_t callback_number)
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
            // 删除对应编号的回调器
            _listeners[ topic ].RemoveCallback(callback_number);
        }
    }

    void Center::RemoveServer(const std::string &topic)
    {
        ReaderLocking(_servers_keeper)
        {
            if (_servers.find(topic) == _servers.end())
            {
                return;
            }
        }

        WriterLocking(_servers_keeper)
        {
            // 卸载服务器
            _servers[ topic ].UnsetService();
        }
    }

    void Center::TopicOffline(const std::string &channel)
    {
        WriterLocking(_listeners_keeper)
        {
            _listeners.erase(channel);
        }
    }

    void Center::ServiceOffline(const std::string &channel)
    {
        WriterLocking(_servers_keeper)
        {
            _servers.erase(channel);
        }
    }
}   // namespace poker::databus::this_process
