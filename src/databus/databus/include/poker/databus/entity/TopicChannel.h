//
// Created by zyk on 24-6-4.
//

#pragma once


namespace poker::databus
{
   /**
   * @brief 定义一个 topic channel，由于每个 channel 与一种通信内容类型
   * 绑定，因此，定义时需指定通信内容的类型。
   * @tparam TMessage 通信内容
   * @tparam Tag 用户自定义标签，可不指定。
   *
   * @example
   * 以下示范如何定义一个 topic channel 标签类型，所用命名空间并不影响最终实现
   *
   * namespace tag
   * {
   *     struct MyTag1 {};
   *     struct MyTag2 {};
   * }
   *
   * namespace channel
   * {
   *     // 定义了一个 int 类型的 topic 通道。
   *     using MyChannel1 = databus::TopicChannel<int, tag::MyTag1>;
   *
   *     // 使用不同的标签，定义另外一个同类型的 topic 通道，
   *     // 最终在 databus_impl 中，可为这两者定义不同的通信实现。
   *     using MyChannel2 = databus::TopicChannel<int, tag::MyTag2>;
   *
   *     // 若标签类型相同，但通信内容不同，最终产生的 topic 通道仍然是不同的。
   *     using MyChannel3 = databus::TopicChannel<double, tag::MyTag2>;
   *
   *     // 若使用上无需区分同一种内容类型、不同的通信实现，也可以不指定标签。
   *     using MyChannel4 = databus::TopicChannel<int>;
   *     using MyChannel5 = databus::TopicChannel<double>;
   * }
   */
    template < class TMessage, class TTag = void >
    struct TopicChannel
    {
        using Tag         = TTag;
        using MessageType = TMessage;
    };
}   // namespace poker::databus