//
// Created by zyk on 2024-6-4 on 22-7-14.
//

#include <poker/chrono.h>
#include <poker/param.h>
#include <poker/type_traits.h>

#include <poker/databus/mqtt/module/Center.h>


namespace poker::databus::mqtt
{
    constexpr const char *param_key = "poker.databus.mqtt.Center";

    bool Center::Read(Center::Param &param)
    {
        return poker::param::Read(param_key, param);
    }

    bool Center::Write(const Center::Param &param)
    {
        return poker::param::Write(param_key, param);
    }

    void Center::Setup()
    {
        // 读取 mqtt 参数
        Read(_param);

        // 设置本进程内使用的 topic 的响应 topic 名称前缀
        _response_topic_prefix = _param.response_topic_prefix + std::to_string(poker::chrono::us()) + "/";

        // 准备各类消息的分发模块
        _listener_center.Prepare(this);
        _caller_center.Prepare(this, CallerCenter::Config {.max_calls_per_channel = _param.max_calls_per_channel,
                                                           .timeout               = _param.call_timeout});
        _server_center.Prepare(this);
        _server_center.SetParallelRunning(_param.max_parallel_serving_threads);

        // 启动各类消息的分发模块
        _listener_center.Start();
        _caller_center.Start();
        _server_center.Start();

        // 初始化 mqtt 客户端
        ResetMqttAndConnect();

        // 启动消息处理主循环
        poker::thread::Loop::Start();
    }

    void Center::Shutdown()
    {
        // 关闭主循环
        thread::Loop::Stop(false);

        // 断开与 mqtt 服务器的连接
        try
        {
            _core_ptr->stop_consuming();
            _core_ptr->disconnect();
        }
        catch (const std::exception &ec)
        {
            std::cout << ec.what() << std::endl;
        }

        // 关闭各类消息的分发模块
        _listener_center.Stop(false);
        _caller_center.Stop(false);
        _server_center.Stop(false);

        // 等待全部分发模块退出
        _caller_center.Wait();
        _server_center.Wait();
        _listener_center.Wait();

        // 等待主循环退出
        thread::Loop::Wait();
    }

    void Center::TopicOffline(const std::string &topic)
    {
        _listener_center.Offline(topic);
    }

    void Center::ServiceOffline(const std::string &service)
    {
        _server_center.UnServe(service);
    }

    const std::string &Center::GetResponseTopicPrefix() const
    {
        return _response_topic_prefix;
    }

    void Center::Subscribe(const std::string &topic, int qos)
    {
        Locking(_reconnection_list)
        {
            try
            {
                // 获取 mqtt 客户端
                ::mqtt::async_client_ptr client_ptr = GetMqttClientPtr();

                // 仅当 mqtt 客户端初始化时进行处理
                if (client_ptr != nullptr)
                    client_ptr->subscribe(topic, qos);
            }
            catch (const std::exception &ec)
            {
                std::cout << ec.what() << std::endl;
            }

            // 记录该 topic 的订阅信息，用于 mqtt 重连或者初始化时进行订阅
            _reconnection_list[ topic ].qos = qos;
        }
    }

    void Center::Unsubscribe(const std::string &topic)
    {
        Locking(_reconnection_list)
        {
            // 获取 mqtt 客户端
            ::mqtt::async_client_ptr client_ptr = GetMqttClientPtr();

            // 仅当 mqtt 客户端初始化时进行处理
            if (client_ptr != nullptr)
                client_ptr->unsubscribe(topic);

            // 删除该 topic 的订阅信息
            _reconnection_list.erase(topic);
        }
    }

    void Center::Publish(::mqtt::message_ptr msg)
    {
        // 获取 mqtt 客户端
        ::mqtt::async_client_ptr client_ptr = GetMqttClientPtr();

        // mqtt 未初始化时，不进行处理
        if (client_ptr == nullptr)
            return;

        // 本函数为功能接口函数，由调用者处理异常
        client_ptr->publish(std::move(msg));
    }

    void Center::OnRun(nox::thread::Loop *)
    {
        // step 0: 检查是否与 broker 断开连接，若是，则进行重连接。
        if (not _core_ptr->is_connected())
        {
            std::cout << "disconnect with broker at " << _core_ptr->get_server_uri();

            // 记录重连时间
            nox::chrono::Clock clock;
            clock.Start();

            // 持续重连，直至程序退出
            while (nox::this_process::Ok())
            {
                if (Reconnect())
                    break;

                // 一小段时间休眠，避免频繁尝试
                nox::chrono::Sleep(noxu::sec(1.0));

                // 若重连时间超时，则尝试新建 mqtt 对象，重新初始化一切
                if (clock.Watch() > _param.reconnect_timeout)
                {
                    ResetMqttAndConnect();
                    break;
                }
            }

            // 经过上述流程之后，mqtt 客户端仍然无法连接，则结束后续处理
            if (not _core_ptr->is_connected())
                return;
        }

        // step 1: 取出当前收到的一份数据，使用非阻塞的方法实现。
        //         若数据指针为空，则该周期立即结束。
        ::mqtt::const_message_ptr msg_ptr;

        if (not _core_ptr->try_consume_message_for(&msg_ptr, std::chrono::seconds(1)))
            return;

        if (msg_ptr == nullptr)
            return;

        // step 2: 分析收到的数据是否是服务器的响应数据，还是普通的监听型数据，
        //         将它们提交到不同分发中心进一步分发。
        const ::mqtt::properties &properties    = msg_ptr->get_properties();
        const std::string        &request_topic = msg_ptr->get_topic();

        const std::size_t response_prefix_length = _response_topic_prefix.length();

        if (request_topic.length() >= response_prefix_length and
            request_topic.substr(0, response_prefix_length) == _response_topic_prefix)
        {
            // 拥有 response topic prefix 的消息，将交由调用者中心分发
            _caller_center.Provide(std::move(msg_ptr));
        }
        else if (properties.contains(::mqtt::property::RESPONSE_TOPIC))
        {
            // 拥有 response topic 属性设置、correlation data 属性设置的消息，
            // 认为是 request topic，由服务器中心分发
            _server_center.Provide(std::move(msg_ptr));
        }
        else
        {
            // 其他普通消息，均当做普通输入数据处理，由监听者 中心分发
            _listener_center.Provide(std::move(msg_ptr));
        }
    }

    void Center::ResetMqttAndConnect()
    {
        // 设置 mqtt 客户端 id
        const std::string client_id =
                nox::this_process::name() + nox::identity::Get().agent_id + std::to_string(nox::chrono::us());

        Locking(_core_ptr)
        {
            _core_ptr = nox::New< ::mqtt::async_client >(_param.server_address, client_id,
                                                         ::mqtt::create_options(MQTTVERSION_5));
        }

        // 循环尝试连接服务器，直到连接成功
        _core_ptr->start_consuming();

        while (nox::this_process::Ok())
        {
            if (Connect())
                break;

            nox::chrono::Sleep(noxu::sec(1.0));
        }
    }

    bool Center::Connect()
    {
        // 准备连接服务器的参数
        ::mqtt::connect_options connect_options = ::mqtt::connect_options_builder()
                                                          .mqtt_version(MQTTVERSION_5)
                                                          .clean_start(true)
                                                          .automatic_reconnect(false)
                                                          .user_name("poker")
                                                          .finalize();

        // 第一次连接 mqtt 服务器
        try
        {
            if (not _core_ptr->connect(connect_options)->wait_for(std::chrono::seconds(10)))
            {
                nox_error "fail to connect with broker at " << _core_ptr->get_server_uri();
                return false;
            }

            nox_info "connect with broker at " << _core_ptr->get_server_uri();
            return true;
        }
        catch (std::exception &ec)
        {
            nox_error ec.what();
            return false;
        }
    }

    bool Center::Reconnect()
    {
        try
        {
            // 尝试与 broker 重新连接
            if (not _core_ptr->reconnect()->wait_for(std::chrono::seconds(10)))
            {
                nox_error "fail to reconnect with broker at " << _core_ptr->get_server_uri();
                return false;
            }

            nox_info "reconnect with broker at " << _core_ptr->get_server_uri();

            // 重新订阅消息
            Locking(_reconnection_list)
            {
                for (auto &[ topic, conn ] : _reconnection_list)
                {
                    _core_ptr->subscribe(topic, conn.qos);
                }
            }

            return true;
        }
        catch (const std::exception &ec)
        {
            nox_error ec.what();
            return false;
        }
    }

    ::mqtt::async_client_ptr Center::GetMqttClientPtr() const
    {
        Locking(_core_ptr) return (const ::mqtt::async_client_ptr &) _core_ptr;
    }
}   // namespace poker::databus::mqtt
