//
// Created by zyk on 2024-6-4 on 22-7-15.
//

#include <poker/databus/mqtt/entity/CallChannel.h>


namespace poker::databus::mqtt
{
    CallChannel::CallChannel(ICore * core_ptr, CallChannel::Config config)
        : _core_ptr(core_ptr), _config(std::move(config))
    {
        // 使用 response topic 统一前缀，设置统一答复用的 topic
        _response_topic = core_ptr->GetResponseTopicPrefix() + _config.request_topic;

        // 初始化调用上下文
        _call_contexts.resize(_config.max_calls_count);

        // 设置全部上下文为可用状态
        for(std::size_t idx = 0; idx < _config.max_calls_count; ++idx)
            _available_context_indexes.enqueue(idx);

        // 可用数量设置为最大
        _have_available_context.Reset(_config.max_calls_count);
    }

    nox::expected<std::string> CallChannel::Call(std::string request_data)
    {
        // 剩余超时时间
        nox::unit::Time timeout = _config.timeout;

        // 准备一个时钟，统计每一步等待的耗时，以控制整体的等待时间受限
        nox::chrono::Clock clock;
        clock.Start();

        // step 1: 等待队列可用；若超时了仍然不可用，则返回失败
        if (not _have_available_context.Acquire(timeout))
            return nox::expected_error();

        // 更新超时时间
        timeout -= clock.Interval();

        // step 2: 取出一个可用的调用上下文
        std::size_t context_index = -1;

        if (not _available_context_indexes.try_dequeue(context_index))
            nox_no_impl();

        CallContext & call_context = _call_contexts[context_index];

        // step 3: 更新并获取 correlation 数，用于匹配响应数据
        long long correlation_number = ++call_context.correlation_number;

        // 设置 correlation 数据，使用上下文索引与该 correlation 数共同构成
        std::string correlation_data = std::to_string(context_index) + "/" + std::to_string(correlation_number);

        // step 4: 构建该 mqtt 消息
        ::mqtt::properties props
            {
                { ::mqtt::property::RESPONSE_TOPIC,   _response_topic },
                { ::mqtt::property::CORRELATION_DATA, correlation_data}
            };

        ::mqtt::message_ptr msg = ::mqtt::message_ptr_builder()
            .topic(_config.request_topic)
            .payload(std::move(request_data))
            .qos(1)
            .properties(std::move(props))
            .finalize();

        // step 5: 重置响应数据容器，发送请求数据
        call_context.message.Ignore();

        try
        {
            _core_ptr->Publish(std::move(msg));
        }
        catch (const std::exception & ec)
        {
            nox_warn "service call fail: " << ec.what();
        }

        // step 6: 在时间限制内，等待响应数据，并取出等待结果
        auto response_result =
            call_context.message.Wait(timeout) ?
            nox::expected<std::string>(nox::expected_value(call_context.message.Read())) :
            nox::expected<std::string>(nox::expected_error());

        // step 7: 等待结束后，归还该调用上下文
        _available_context_indexes.enqueue(context_index);
        _have_available_context.Release();

        // step 8: 返回响应结果
        return response_result;
    }

    void CallChannel::NotifyResponse(const ::mqtt::const_message_ptr & response_msg)
    {
        // step 1: 解析该响应数据的 correlation data，找出它属于哪一个调用上下文，
        //         以及对应的 correlation number
        std::string correlation_data;

        if (const ::mqtt::properties & properties = response_msg->get_properties();
            properties.contains(::mqtt::property::CORRELATION_DATA))
        {
            correlation_data = ::mqtt::get<std::string>(
                response_msg->get_properties(), ::mqtt::property::CORRELATION_DATA
            );
        }

        std::vector<std::string> correlation_data_items =
            utils::SplitAndTrim(correlation_data, '/');

        // 若格式不符合预期，则忽略该数据
        if (correlation_data_items.size() != 2)
            return;

        std::size_t context_index     = -1;
        long long   correlation_numer = -1;

        try
        {
            context_index     = std::stoull(correlation_data_items[0]);
            correlation_numer = std::stoll (correlation_data_items[1]);
        }
        catch (...)
        {
            return;
        }

        // step 2: 检查该消息是否命中调用上下文中的期待响应，
        //         若是，则设置响应数据
        if (context_index < _call_contexts.size())
        {
            CallContext & call_context = _call_contexts[context_index];

            if (call_context.correlation_number == correlation_numer)
                call_context.message.Write(response_msg->get_payload_str());
        }
    }
}

namespace poker::databus::mqtt
{
    // 本模块的实现能保证不会发生任何有意义的拷贝。
    // 因此这里不做任何事情。
    CallChannel::CallContext::CallContext(CallChannel::CallContext &&) noexcept
    {
    }
}
