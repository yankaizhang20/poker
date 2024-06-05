//
// Created by zyk on 2024-6-4 on 22-7-14.
//

#include <poker/databus/mqtt/component/CallerCenter.h>
#include <nox/thread/message_loop/Processor.h>


namespace poker::databus::mqtt
{
    void CallerCenter::Prepare(ICore * core_ptr, CallerCenter::Config config)
    {
        _core_ptr = core_ptr;
        _config   = std::move(config);
        _call_channels.SetUnlimitedSize();
    }

    nox::expected<std::string> CallerCenter::Call(const std::string & request_topic, std::string request_data)
    {
        // step 1: 根据请求 topic 名，取出对应的调用通道（不用加锁）。
        //         若该通道还未初始化，则创建之
        CallChannel::Config possible_new_channel_config
            {
                .max_calls_count = _config.max_calls_per_channel,
                .request_topic   = request_topic,
                .timeout         = _config.timeout
            };

        auto [channel_ptr, is_new_channel] =
            _call_channels.DataUnsafePtrOrNew(request_topic, _core_ptr, possible_new_channel_config);

        // 若获取失败（channel 新建失败），说明内存不足
        if (channel_ptr == nullptr)
            nox_no_impl("memory insufficient for new caller channel !");

        // step 2: 若通道是新建的，则向 mqtt 注册响应数据的监听；
        //         该监听不能被用户主动卸载，但也没有关系。
        if (is_new_channel)
        {
            _core_ptr->Subscribe(_core_ptr->GetResponseTopicPrefix() + request_topic, 1);
        }

        // step 3: 向给通道，发起请求，并返回结果
        return channel_ptr->Call(std::move(request_data));
    }

    void CallerCenter::OnSetup(nox::thread::MessageLoopConfig<::mqtt::const_message_ptr> & config)
    {
        config.buffer_size      = config::MaxCallerBufferSize;
        config.throughput       = 1;
        config.parallel_running = std::max<std::size_t>(1, config::MaxCallerBufferSize / 4);
    }

    void CallerCenter::OnExecute(MessageBuffer<::mqtt::const_message_ptr> & buffer)
    {
        // step 1: 取出消息数据
        nox_assert(buffer.size() == 1);
        ::mqtt::const_message_ptr msg = buffer[0];

        // step 2: 从接收到的 response topic 中，根据 response topic prefix，解析出
        //         原来的 request topic
        std::string request_topic = msg->get_topic().substr(_core_ptr->GetResponseTopicPrefix().size());

        // step 3: 取出该 topic 对应的请求通道；若不存在该通道，则什么事情都不发生。
        CallChannel * call_channel = _call_channels.DataUnsafePtr(request_topic);

        if (call_channel == nullptr)
            return;

        // step 4: 向该请求通道提交响应数据
        call_channel->NotifyResponse(msg);
    }
}
