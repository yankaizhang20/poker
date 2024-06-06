//
// Created by zyk on 2024-6-6
//

#pragma once

#include <optional>

#include "./callback/name_hash.h"


namespace poker::databus::details
{
    /**
     * @brief 使用字符串管理的、有序的容器，来存放多个回调函数的处理结果。
     * 各个回调函数的结果可能为空，说明它们未被调用。
     */
    template < class T >
    class CallbackResult
    {
    public:
        /**
         * @brief 标识回调结果是否被拦截。
         */
        bool intercepted = false;

    private:
        std::map< std::size_t, std::optional< T > > data_;
    };

    template <>
    class CallbackResult< void >
    {
    };
}   // namespace poker::databus::details
