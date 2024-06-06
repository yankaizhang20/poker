//
// Created by zyk on 24-6-4.
//

#pragma once

#include <poker/thread/lock.h>

#include "./databus/entity/AuxDeleter.h"
#include "./databus/entity/ServiceChannel.h"
#include "./databus/entity/TopicChannel.h"
#include "./databus/entity/XServiceChannel.h"
#include "./databus/extern_impl.h"

#include "./databus/databus_impl.h"


/**
 * @brief databus 为模块间通信的中间件，它仅提供了统一的通信接口，但没有定义具体的通信行为，
 * 所有的通信行为需要应用层去实现，因此可以根据不同的通信要求进行不同的实现。
 *
 * @example
 * // 以下注释演示如何为不同类型、不同通道，定义具体的通信实现（包括发送、监听、服务、调用、关闭），
 * // 而 databus 使用 API 可见不同函数各自的介绍。
 *
 * // 假设需要发送、监听的类型如下：
 * struct MyType {};
 *
 * // 假设服务调用过程所需请求数据、响应数据类型如下：
 * struct MyRequest  {};
 * struct MyResponse {};
 *
 * // 使用 databus，需要定义通信通道标签，该通道标签支持设置自定义标签，
 * // 在本工程内，自定义标签均放在 poker::tag 命名空间下：
 * namespace poker::tag
 * {
 *     struct MyTag {};
 * }
 *
 * // 为消息的发送、监听，定义 TopicChannel 标签，
 * // 在本工程中，所有的通道标签均放置在 poker::channel 命名空间下：
 * namespace poker::channel
 * {
 *     // 使用具体通信类型定义通道：
 *     using MyTopicChannel = databus::TopicChannel<MyType>;
 *
 *     // 也可以配合一个自定义标签，定义另外一个完全不同的通道：
 *     using AnotherTopicChannel = databus::TopicChannel<MyType, tag::MyTag>;
 *
 *     // 注意，若使用别的类型，配合相同的标签，定义得到的通道也仍然是完全不同的另外一个通道。
 * }
 *
 * // 为服务过程的服务注册、服务调用，定义 ServiceChannel 标签，
 * // 同样在 poker::channel 命名空间下定义：
 * namespace poker::channel
 * {
 *     // 使用具体通信类型定义通道：
 *     using MyServiceChannel = databus::ServiceChannel<MyRequest, MyResponse>;
 *
 *     // 也可以配合一个自定义标签，定义另外一个完全不同的通道：
 *     using AnotherServiceChannel = databus::ServiceChannel<MyRequest, MyResponse, tag::MyTag>;
 * }
 *
 * // 目前，databus 支持字符串通道类型，也即，每个 channel 将被翻译为一个字符串，提供给通信实现参考。
 * // 我们需要在 poker::databus_impl 命名空间下，为不同通信类型、不同标签类型，实现通信函数：
 * namespace poker::databus_impl
 * {
 *     // 定义 MyType 数据类型、不使用自定义标签的发送过程
 *     template<>
 *     void Send<MyType>(const std::string & channel, const MyType & data)
 *     {
 *     }
 *
 *     // 定义 MyType 数据类型、使用自定义标签的发送过程
 *     template<>
 *     void Send<MyType, tag::MyTag>(const std::string & channel, const MyType & data)
 *     {
 *     }
 *
 *     // 定义 MyType 数据类型、不使用自定义标签的监听注册过程
 *     template<>
 *     AuxDeleter Listen<MyType>(const std::string & channel, const Handler<MyType> & handler)
 *     {
 *         return AuxDeleter{ .deleter_caller = []() {} }; // 根据具体实现需要，设置对应卸载器逻辑
 *     }
 *
 *     // 定义 MyType 数据类型、使用自定义标签的监听注册过程
 *     template<>
 *     AuxDeleter Listen<MyType, tag::MyTag>(const std::string & channel, const Handler<MyType> & handler)
 *     {
 *         return AuxDeleter{ .deleter_caller = []() {} };
 *     }
 *
 *     // 定义 MyRequest 请求数据类型、MyResponse 响应数据类型、不使用自定义标签的服务注册过程
 *     template<>
 *     AuxDeleter Serve<MyRequest, MyResponse>(
 *         const std::string & channel, const Server<MyRequest, MyResponse> & server)
 *     {
 *         return {}; // 根据具体实现需要，设置对应卸载器逻辑
 *     }
 *
 *     // 定义 MyRequest 请求数据类型、MyResponse 响应数据类型、使用自定义标签的服务注册过程
 *     template<>
 *     AuxDeleter Serve<MyRequest, MyResponse, tag::MyTag>(
 *         const std::string & channel, const Server<MyRequest, MyResponse> & server)
 *     {
 *         return {}; // 根据具体实现需要，设置对应卸载器逻辑
 *     }
 *
 *     // 定义 MyRequest 请求数据类型、MyResponse 响应数据类型、不使用自定义标签的服务调用过程
 *     nox::expected<MyResponse> Call<MyRequest, MyResponse>(
 *         const std::string & channel, const MyRequest & request)
 *     {
 *         return nox::expected_value( MyResponse{} ); // 根据具体实现，可选择返回成功或失败。
 *         // return nox::expected_error();
 *     }
 *
 *     // 定义 MyRequest 请求数据类型、MyResponse 响应数据类型、使用自定义标签的服务调用过程
 *     nox::expected<MyResponse> Call<MyRequest, MyResponse, tag::MyTag>(
 *         const std::string & channel, const MyRequest & request)
 *     {
 *         return nox::expected_value( MyResponse{} ); // 根据具体实现，可选择返回成功或失败。
 *         // return nox::expected_error();
 *     }
 *
 *     // 定义指定通道的统一卸载流程，不分发送或监听，不分服务注册或服务调用：
 *     template<>
 *     void Offline<channel::MyTopicChannel>(const std::string & channel)
 *     {
 *     }
 *
 *     template<>
 *     void Offline<channel::MyServiceChannel>(const std::string & channel)
 *     {
 *     }
 *
 *     // 可以为某个通道标签，定义它被翻译为字符串后的内容
 *     template<>
 *     std::string channel_name<channel::MyTopicChannel> = "/my/channel/name";
 *
 *     // 若未定义，则默认使用通道标签的 typeid 值。
 * }
 *
 *
 * @note 最终编译为可执行程序时，仅需要将使用者的库和实现者的库链接在一起即可。
 */
namespace poker::databus
{
    /**
     * @brief 向指定通道发送指定类型的消息。
     * @tparam TopicChannel 使用 poker::databus::TopicChannel<> 定义的通道标签类型
     * @param data 待发送的数据，其类型由 TopicChannel 定义。
     *
     * @example
     * // 使用本发送消息的函数，需要定义 TopicChannel 标签类型，一个示例如下：
     *
     * // 假设需要发送的消息类型如下：
     * struct MyType { int a; bool b; };
     *
     * namespace tag
     * {
     *     struct MyTag {};
     * }
     *
     * // 使用目标类型定义一个 TopicChannel 标签，在本工程中，所有的通道标签均
     * namespace channel
     * {
     *     using MyChannel = databus::TopicChannel<MyType>;
     *
     *     // 若需要为同一种类型实现不同通信方法，可以再指定指定一个自定义标签：
     *     using AnotherChannel = databus::TopicChannel<MyType, tag::MyTag>;
     * }
     *
     * // 使用该通道，发送该通道约定的数据：
     * void fn()
     * {
     *     MyType msg;
     *     poker::databus::Send<channel::MyChannel>(msg);
     * }
     */
    template < class TopicChannel >
    void Send(const typename TopicChannel::MessageType &data)
    {
        databus_impl::Send< typename TopicChannel::MessageType, typename TopicChannel::Tag >(
                details::ChannelImpl<>::value< TopicChannel >(), data);
    }

    /**
     * @brief 监听指定通道中的指定类型数据
     * @tparam TopicChannel 使用 poker::databus::TopicChannel<> 定义的通道标签类型
     * @param handler 数据接收时触发的回调处理器，数据类型由 TopicChannel 定义。
     * @return 回调处理器的卸载入口，用于 databus::Aux 对象管理。
     *
     * @example
     * // 使用本消息监听函数，需要定义 TopicChannel 标签类型，可以参考 @fn Send() 函数的介绍。
     * // 假设已经定义 channel::MyChannel 标签类型，使用本函数进行监听的例子如下：
     *
     * void fn()
     * {
     *     using namespace poker;
     *
     *     // 启动监听，需传入一个 void(T) 签名的监听器：
     *     databus::Listen<MyChannel>( [](MyType msg) {} );
     *
     *     // 取消监听，需要使用相同的通道标签：
     *     databus::Offline<MyChannel>();
     *
     *     // 也可以在启动监听时，将卸载接口挂载到一个 Aux 对象上，并随后使用 Aux 对象取消监听：
     *     databus::Aux aux;
     *     aux += databus::Listen<MyChannel>( [](MyType msg) {} );
     *
     *     // 通过 Aux 对象取消监听：
     *     aux.Offline();
     *
     *     // 或待 Aux 对象析构时，自动取消。
     *     // 若不想自己定义 Aux 对象，也可以使用 databus 提供的标签化全局 Aux 对象：
     *     databus::aux<MyTag> +=  databus::Listen<MyChannel>( [](MyType msg) {} );
     *     databus::aux<MyTag>.Offline();
     * }
     */
    template < class TopicChannel >
    AuxDeleter Listen(const Handler< typename TopicChannel::MessageType > &handler)
    {
        if (handler != nullptr)
            return databus_impl::Listen< typename TopicChannel::MessageType, typename TopicChannel::Tag >(
                    details::ChannelImpl<>::value< TopicChannel >(), handler);
        else
            return {};
    }

    /**
     * @brief 发起指定通道的服务调用，输入请求数据，得到响应数据。
     * @tparam ServiceChannel 使用 poker::databus::ServiceChannel<> 定义的通道标签类型
     * @param req 请求数据，其类型由 ServiceChannel 定义。
     * @return 响应数据，其类型由 ServiceChannel 定义。
     *
     * @example
     * // 使用本函数发起服务调用，需要指定 ServiceChannel，其定义方法可参考 @fn Serve() 的介绍。
     * // 假设已经定义 channel::MyChannel 标签类型，使用本函数进行服务调用的例子如下：
     *
     * void fn()
     * {
     *     using namespace poker;
     *
     *     // 准备好请求数据：
     *     MyRequest request;
     *
     *     // 发起服务调用
     *     std::optional<MyResponse> response = databus::Call<channel::MyChannel>(request);
     *
     *     // 分析响应结果：
     *     if (response.has_value())
     *     {
     *         MyResponse data = response.value();
     *     }
     *     else
     *     {
     *     }
     * }
     */
    template < class ServiceChannel >
    std::optional< typename ServiceChannel::ResponseType > Call(const typename ServiceChannel::RequestType &req)
    {
        return databus_impl::Call< typename ServiceChannel::RequestType, typename ServiceChannel::ResponseType,
                                   typename ServiceChannel::Tag >(
                details::ChannelImpl< typename ServiceChannel::ImplType >::template value< ServiceChannel >(), req);
    }

    /**
     * @brief 设置指定通道的服务器，用于处理其他模块的请求，并返回响应数据。
     * @tparam ServiceChannel 使用 poker::databus::ServiceChannel<> 定义的通道标签类型
     * @param server 提供服务过程的函数，其处理的请求、响应数据类型，由 ServiceChannel 定义。
     * @return 服务函数的卸载入口，用于 databus::Aux 对象管理。
     *
     * @example
     * // 使用本服务注册函数，需要定义 ServiceChannel 标签类型，一个示例如下：
     *
     * // 假设请求、响应数据的类型如下：
     * struct MyRequest  {};
     * struct MyResponse {};
     *
     * // 定义一个自定义标签
     * namespace tag
     * {
     *     struct MyTag {};
     * }
     *
     * // 使用目标类型定义一个 ServiceChannel
     * namespace channel
     * {
     *     using MyChannel = databus::ServiceChannel<MyRequest, MyResponse>;
     *
     *     // 若需要为同一种类型实现不同通信方法，可以再指定指定一个自定义标签：
     *     using AnotherChannel = databus::ServiceChannel<MyRequest, MyResponse, tag::MyTag>;
     * }
     *
     * // 使用该通道，注册服务函数：
     * void fn()
     * {
     *     using namespace poker;
     *
     *     // 注册服务函数：
     *     databus::Serve<channel::MyChannel>(
     *         [](const MyRequest & request, MyResponse & response) -> bool
     *         {
     *             return true; // 返回 true 表示响应有效，否则反之。
     *         }
     *     );
     *
     *     // 使用同一个通道，卸载服务函数：
     *     databus::Offline<channel::MyChannel>();
     *
     *     // 也可以使用 Aux 对象来管理本函数返回的服务卸载接口，使用方式可参考 @fn Listen() 函数的介绍。
     * }
     */
    template < class ServiceChannel >
    AuxDeleter
    Serve(const Server< typename ServiceChannel::RequestType, typename ServiceChannel::ResponseType > &server)
    {
        if (server != nullptr)
            return databus_impl::Serve< typename ServiceChannel::RequestType, typename ServiceChannel::ResponseType,
                                        typename ServiceChannel::Tag >(
                    details::ChannelImpl< typename ServiceChannel::ImplType >::template value< ServiceChannel >(),
                    server);
        else
            return {};
    }

    /**
     * @brief 关闭指定通道的通信（包括 TopicChannel 与 ServiceChannel）
     * @tparam Channel
     */
    template < class Channel >
    void Offline()
    {
        databus_impl::Offline< Channel >(details::ChannelImpl<>::value< Channel >());
    }

    /**
     * @brief 辅助管理监听卸载器、服务卸载器，提供统一卸载 API，以及实现析构自动卸载。
     */
    class Aux : public poker::thread::MutexLock
    {
    public:
        Aux &operator+= (AuxDeleter x);

        void Offline();

        ~Aux() override;

    private:
        std::vector< AuxDeleter > _data;
    };

    /**
     * @brief 全局标签化的 Aux 对象，提供方便的使用。
     */
    template < class Tag >
    static Aux aux;
}   // namespace poker::databus