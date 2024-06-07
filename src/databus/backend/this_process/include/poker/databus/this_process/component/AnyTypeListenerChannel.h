//
// Created by zyk on 2024-6-6.
//

#pragma once

#include <any>
#include <functional>

#include <poker/databus.h>

#include "./details/Callback.h"
#include "./details/CallbackConfig.h"


namespace poker::databus
{
    class AnyTypeListenerChannel
    {
    private:
        // 已知具体类型的监听上下文信息
        template < class T >
        struct Context
        {
            details::Callback< void(T) > callback;
        };

    public:
        /**
         * @return 是否初始化通道
         */
        bool IsInitialized() const;

        /**
         * @brief 以指定的类型，初始化本监听通道。
         */
        template < class T >
        void Prepare(const details::CallbackConfig &config)
        {
            // 初始化指定类型的上下文，不允许初始化两次（也即，一个通道仅可处理确定的、唯一的数据类型）
            if (_core.has_value())
                poker_never("initialize listener channel with different types");

            // 初始化上下文信息为具体的类型
            _core.emplace< std::shared_ptr< Context< T > > >(std::make_shared< Context< T > >());

            // 上下文信息
            Context< T > &context = GetContext< T >();

            // 设置回调的策略
            context.callback.EnableConcurrentCalls(false);
            context.callback.EnableParallelCallbacks(true);
            context.callback.EnableDiscardCalls(config.enable_discard_calls);

            // 设置一些有用的闭包
            _wait_caller = [ & ]() { GetContext< T >().callback.Wait(); };

            _remove_callback_caller = [ this ](std::size_t n) { GetContext< T >().callback.RemoveCallback(n); };

            _empty_caller = [ this ]() { return GetContext< T >().callback.Empty(); };
        }

        /**
         * @brief 添加一个回调函数，类型需要与本类的初始化类型匹配，否则不会有任何效果。
         * @return 该回调的唯一索引编号
         */
        template < class T >
        std::optional< std::size_t > AddCallback(Handler< T > handler)
        try
        {
            std::size_t callback_number = _callback_unique_number++;

            GetContext< T >().callback.AddCallback(callback_number, std::move(handler));

            return callback_number;
        }
        catch (...)
        {
            return {};
        }

        /**
         * @brief 删除指定编号的回调函数
         */
        void RemoveCallback(std::size_t callback_number);

        /**
         * @brief 通知新消息到来，触发异步的回调过程
         */
        template < class T >
        void Notify(const T &msg)
        try
        {
            GetContext< T >().callback.AsyncCall(msg);
        }
        catch (...)
        {
            // Do nothing here ...
        }

        /**
         * @brief 等待所有回调流程结束
         */
        void Wait() const;

        /**
         * @return 本监听通道是否没有任何回调器
         */
        bool Empty() const;

    private:
        template < class T >
        Context< T > &GetContext()
        {
            return *(std::any_cast< std::shared_ptr< Context< T > > & >(_core));
        }

    private:
        // 用于唯一索引某个 callback
        static std::atomic_size_t _callback_unique_number;

    private:
        // 存放某种类型的上下文；同一时间，一个 channel 仅能放置一个类型
        std::any _core;

        // 调用上下文中 callback 对象的 RemoveCallback() 函数的闭包
        std::function< void(std::size_t n) > _remove_callback_caller;

        // 用于调用上下文中 callback 对象的 Wait() 函数的闭包
        std::function< void() > _wait_caller;

        // 用于调用上下文中 callback 对象的 Empty() 函数的闭包
        std::function< bool() > _empty_caller;
    };
}   // namespace poker::databus
