//
// Created by zyk on 2024-6-4 on 22-7-14.
//

#include <poker/databus/mqtt/component/ListenerCenter.h>


namespace poker::databus::mqtt
{
    void ListenerCenter::Prepare(ICore * core_ptr)
    {
        _core_ptr = core_ptr;
        _listeners.SetUnlimitedSize();
    }

    void ListenerCenter::Offline(const std::string & topic)
    {
        _core_ptr->Unsubscribe(topic);
        _listeners.Delete(topic);
    }

    void ListenerCenter::Offline(const std::string & topic, std::size_t callback_number)
    {
        // 取出 topic 的回调通道
        SerializableListenerChannel * channel_ptr = _listeners.DataPtr(topic);

        // 若没有这个通道，则立即返回
        if (channel_ptr == nullptr)
            return;

        // 删除对应编号的回调函数
        channel_ptr->RemoveCallback(callback_number);

        // 若该通道没有回调函数，则卸载该通信通道；
        // 否则，释放该通道的锁后，结束该流程。
        if (channel_ptr->Empty())
        {
            _core_ptr->Unsubscribe(topic);
            _listeners.ReleasePtrAndDelete(topic);
        }
        else
        {
            _listeners.ReleasePtr(topic);
        }
    }

    void ListenerCenter::OnSetup(nox::thread::MessageLoopConfig<::mqtt::const_message_ptr> & config)
    {
        config.buffer_size      = config::MaxListenerBufferSize;
        config.throughput       = 1;
        config.parallel_running = std::max<std::size_t>(1, config::MaxListenerBufferSize / 4);
    }

    void ListenerCenter::OnExecute(MessageBuffer<::mqtt::const_message_ptr> & buffer)
    {
        // step 1: 取出消息数据
        nox_assert(buffer.size() == 1);
        ::mqtt::const_message_ptr msg = buffer[0];

        const std::string & request_topic = msg->get_topic();

        // step 2: 寻找该 topic 对应的监听通道；
        //         由于该监听通道可能被别的线程删除，因此需要加锁取出。
        const SerializableListenerChannel * channel_ptr = _listeners.DataConstPtr(request_topic);

        // 若找不到对应的监听通道，则不发生任何事情
        if (channel_ptr == nullptr)
            return;

        // step 3: 通知新消息到达
        channel_ptr->Notify(msg->get_payload_str());

        // step 4: 释放该监听通道的锁
        _listeners.ReleaseConstPtr(request_topic);
    }
}
