//
// Created by zyk on 24-6-6.
//

#pragma once


/**
 * @brief 创建指定的域内 topic 发送实现
 */
#define poker_topic_impl_send(Impl, TopicChannel)                                                           \
    namespace poker::databus_impl                                                                           \
    {                                                                                                       \
        template <>                                                                                         \
        void Send< TopicChannel::MessageType, TopicChannel::Tag >(const std::string               &channel, \
                                                                  const TopicChannel::MessageType &data)    \
        {                                                                                                   \
            Impl::Send(channel, data);                                                                      \
        }                                                                                                   \
    }


/**
 * @brief 创建指定的域内 topic 实现。
 * @example
 * #include <poker/databus/this_process.h>
 *
 * using T1 = poker::databus::TopicChannel<int>;
 *
 * poker_topic_impl(this_process, T1);
 *
 * int main()
 * {
 *     poker::databus::Listen<T1>(
 *         [](int x)
 *         {
 *         }
 *     );
 *
 *     poker::databus::Send<T1>(1);
 *
 *     poker::databus::Offline<T1>();
 *
 *     return 0;
 * }
 */
#define poker_topic_impl(Impl, TopicChannel)                                                     \
    poker_topic_impl_send(Impl, TopicChannel)                                                    \
    namespace poker::databus_impl                                                                \
    {                                                                                            \
        template <>                                                                              \
        AuxDeleter Listen< TopicChannel::MessageType, TopicChannel::Tag >(                       \
                const std::string &channel, const Handler< TopicChannel::MessageType > &handler) \
        {                                                                                        \
            return Impl::Listen(channel, handler);                                               \
        }                                                                                        \
                                                                                                 \
        template <>                                                                              \
        void Offline< TopicChannel >(const std::string &channel)                                 \
        {                                                                                        \
            Impl::TopicOffline(channel);                                                         \
        }                                                                                        \
    }


/**
 * @brief 创建指定的域内 service 实现。
 * @example
 * #include <poker/databus/this_process.h>
 *
 * using S1 = poker::databus::ServiceChannel<int, double>;
 *
 * poker_service_impl(S1);
 *
 * ...
 */
#define poker_service_impl(Impl, ServiceChannel)                                                            \
    namespace poker::databus_impl                                                                           \
    {                                                                                                       \
        template <>                                                                                         \
        std::optional< ServiceChannel::ResponseType >                                                       \
        Call< ServiceChannel::RequestType, ServiceChannel::ResponseType, ServiceChannel::Tag >(             \
                const std::string &channel, const ServiceChannel::RequestType &req)                         \
        {                                                                                                   \
            return Impl::Call< ServiceChannel::RequestType, ServiceChannel::ResponseType >(channel, req);   \
        }                                                                                                   \
                                                                                                            \
        template <>                                                                                         \
        AuxDeleter Serve< ServiceChannel::RequestType, ServiceChannel::ResponseType, ServiceChannel::Tag >( \
                const std::string                                                         &channel,         \
                const Server< ServiceChannel::RequestType, ServiceChannel::ResponseType > &server)          \
        {                                                                                                   \
            return Impl::Serve(channel, server);                                                            \
        }                                                                                                   \
                                                                                                            \
        template <>                                                                                         \
        void Offline< ServiceChannel >(const std::string &channel)                                          \
        {                                                                                                   \
            Impl::ServiceOffline(channel);                                                                  \
        }                                                                                                   \
    }


#define poker_service_impl_by_http(Method, ServiceChannel)                                                         \
    namespace poker::databus_impl                                                                                  \
    {                                                                                                              \
        template <>                                                                                                \
        struct channel_method< ServiceChannel >                                                                    \
        {                                                                                                          \
            using type = databus::http::Method;                                                                    \
        };                                                                                                         \
                                                                                                                   \
        template <>                                                                                                \
        std::optional< ServiceChannel::ResponseType >                                                              \
        Call< ServiceChannel::RequestType, ServiceChannel::ResponseType, ServiceChannel::Tag >(                    \
                const databus::XChannelType &channel, const ServiceChannel::RequestType &req)                      \
        {                                                                                                          \
            return databus_impl::http::Call< channel_method< ServiceChannel >::type, ServiceChannel::RequestType,  \
                                             ServiceChannel::ResponseType >(channel, req);                         \
        }                                                                                                          \
                                                                                                                   \
        template <>                                                                                                \
        AuxDeleter Serve< ServiceChannel::RequestType, ServiceChannel::ResponseType, ServiceChannel::Tag >(        \
                const databus::XChannelType                                               &channel,                \
                const Server< ServiceChannel::RequestType, ServiceChannel::ResponseType > &server)                 \
        {                                                                                                          \
            return databus_impl::http::Serve< channel_method< ServiceChannel >::type, ServiceChannel::RequestType, \
                                              ServiceChannel::ResponseType >(channel, server);                     \
        }                                                                                                          \
        template <>                                                                                                \
        void Offline< ServiceChannel >(const databus::XChannelType &channel)                                       \
        {                                                                                                          \
            databus_impl::http::Offline< channel_method< ServiceChannel >::type >(channel);                        \
        }                                                                                                          \
    }
