//
// Created by zyk on 2024-6-5
//

#include "poker/databus/this_process/module/Center.h"


namespace poker::databus::this_process
{
    Center::Center()
    {
        _listeners.SetUnlimitedSize();
        _servers.SetUnlimitedSize();
    }

    void Center::RemoveCallback(const std::string &topic, std::size_t callback_number)
    {
        // 取出指定的回调管理器，并上锁
        AnyTypeListenerChannel *ptr = _listeners.DataPtr(topic);

        if (ptr == nullptr)
            return;

        // 删除对应编号的回调器
        ptr->RemoveCallback(callback_number);

        // 释放该回调管理器的数据锁
        _listeners.ReleasePtr(topic);
    }

    void Center::RemoveServer(const std::string &topic)
    {
        // 取出指定服务管理器，并上锁
        ServiceWrapper *ptr = _servers.DataPtr(topic);

        if (ptr == nullptr)
            return;

        // 卸载服务器
        ptr->UnsetService();

        // 释放数据锁
        _servers.ReleasePtr(topic);
    }

    void Center::TopicOffline(const std::string &channel)
    {
        _listeners.Delete(channel);
    }

    void Center::ServiceOffline(const std::string &channel)
    {
        _servers.Delete(channel);
    }
}   // namespace poker::databus::this_process
