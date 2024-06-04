//
// Created by zyk on 24-6-4.
//

#pragma once


namespace poker::databus
{
    /**
     * @brief 定义一个 service channel，由于每个 channel 与一种通信内容类型
     * （包含请求、响应的类型）是绑定的，因此，定义时需指定通信内容的类型。
     * @tparam TRequest 请求数据的类型
     * @tparam TResponse 响应数据的类型
     * @tparam Tag 用户自定义标签，可不指定。
     *
     * @example
     * 以下示范如何定义一个 service channel 标签类型
     *
     * namespace tag
     * {
     *     struct MyTag1 {};
     *     struct MyTag2 {};
     * }
     *
     * namespace channel
     * {
     *     // 定义了一个请求类型为 int，响应类型为 bool 的 service 通道。
     *     using MyChannel1 = databus::ServiceChannel<int, bool, tag::MyTag1>;
     *
     *     // 使用不同的标签，定义另外一个同类型的 service 通道，
     *     // 最终在 databus_impl 中，可为这两者定义不同的通信实现。
     *     using MyChannel2 = databus::ServiceChannel<int, bool, tag::MyTag2>;
     *
     *     // 若标签类型相同，但通信内容不同，最终产生的 service 通道仍然是不同的。
     *     using MyChannel3 = databus::ServiceChannel<int, double, tag::MyTag2>;
     *
     *     // 若使用上无需区分同一种内容类型、不同的通信实现，也可以不指定标签。
     *     using MyChannel4 = databus::ServiceChannel<int, bool>
     *     using MyChannel5 = databus::ServiceChannel<int, double>
     *     using MyChannel6 = databus::ServiceChannel<double, bool>
     *     using MyChannel7 = databus::ServiceChannel<bool, int>
     * }
     */
    template < class TRequest, class TResponse, class TTag = void >
    struct ServiceChannel
    {
        using Tag          = TTag;
        using RequestType  = TRequest;
        using ResponseType = TResponse;
        using ImplType     = std::string;
    };
}   // namespace poker::databus