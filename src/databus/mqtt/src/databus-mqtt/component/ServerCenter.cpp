//
// Created by zyk on 2024-6-4 on 22-7-14.
//

#include <poker/databus/mqtt/component/ServerCenter.h>


namespace poker::databus::mqtt
{
    void ServerCenter::Prepare(ICore * core_ptr)
    {
        _core_ptr = core_ptr;
        _servers.SetUnlimitedSize();
    }

    AuxDeleter ServerCenter::Serve(const std::string & service_topic, ServerCenter::Handler handler)
    {
        nox_assert(not service_topic.empty());
        nox_assert(handler != nullptr);

        // step 1: 创建指定服务通道；若指定通道已经存在，则进行报错
        if (_servers.Find(service_topic))
            nox_never("register more than one handlers for one service channel !");

        if (not _servers.New(service_topic, std::move(handler)))
            nox_no_impl("memory insufficient for more server !");

        // step 2: 向 mqtt 注册该 topic 的监听
        _core_ptr->Subscribe(service_topic, 1);

        // step 3: 构建卸载器，并返回
        return AuxDeleter
            {
                .deleter_caller = [this, service_topic]
                {
                    this->UnServe(service_topic);
                }
            };
    }

    void ServerCenter::UnServe(const std::string & service_topic)
    {
        // step 1: 向 mqtt 卸载该 topic 的监听
        _core_ptr->Unsubscribe(service_topic);

        // step 2: 删除该 topic 的 server
        _servers.Delete(service_topic);
    }

    void ServerCenter::OnSetup(nox::thread::MessageLoopConfig<::mqtt::const_message_ptr> & config)
    {
        config.buffer_size      = config::MaxServerBufferSize;
        config.throughput       = 1;
        config.parallel_running = std::max<std::size_t>(1, config::MaxServerBufferSize / 4);
    }

    void ServerCenter::OnExecute(MessageBuffer<::mqtt::const_message_ptr> & buffer)
    {
        // step 1: 取出消息数据
        nox_assert(buffer.size() == 1);
        ::mqtt::const_message_ptr msg = buffer[0];

        const std::string & request_topic = msg->get_topic();

        // step 2: 寻找该请求 topic 对应的服务器；
        //         由于服务器有可能被别的线程删除，因此需要进行加锁取出。
        const Handler * handler = _servers.DataConstPtr(request_topic);

        // 若找不到对应的服务器，则不发生任何事情
        if (handler == nullptr)
            return;

        // step 3: 处理请求数据
        Serving(*handler, msg);

        // step 4: 释放该服务器的锁
        _servers.ReleaseConstPtr(request_topic);
    }

    void ServerCenter::Serving(const ServerCenter::Handler & handler, const ::mqtt::const_message_ptr & request_msg)
    {
        // step 1: 调用 handler 进行处理，获得结果
        nox::expected<std::string> response_opt = handler(request_msg->get_payload_str());

        // 若响应数据为空，则结束处理，也不答复调用者
        if (response_opt.is_error())
            return;

        // step 2: 取出 request topic 中的属性，准备用于答复调用者
        const ::mqtt::properties & request_props = request_msg->get_properties();

        // 设置可能的属性值
        ::mqtt::properties response_props;

        if (request_props.contains(::mqtt::property::CORRELATION_DATA))
        {
            auto correlation_data = ::mqtt::get<std::string>(request_props, ::mqtt::property::CORRELATION_DATA);

            response_props.add(::mqtt::property(::mqtt::property::CORRELATION_DATA, correlation_data));
        }

        // step 3: 答复指定通道的 response
        auto response_topic = ::mqtt::get<std::string>(request_props, ::mqtt::property::RESPONSE_TOPIC);

        ::mqtt::message_ptr reply_msg_ptr =
            ::mqtt::message::create(response_topic, response_opt.value().take(), 1, false, response_props);

        // step 4: 发送 response
        try
        {
            _core_ptr->Publish(std::move(reply_msg_ptr));
        }
        catch (const std::exception & ec)
        {
            nox_error "service response fail: " << ec.what();
        }
    }
}
