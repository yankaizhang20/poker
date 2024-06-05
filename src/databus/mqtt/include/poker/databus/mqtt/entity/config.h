//
// Created by zyk on 2024-6-4 on 22-7-14.
//

#pragma once

#include <cstdint>


namespace poker::databus::mqtt::config
{
    // 同时处理的 caller 的消息最大数量
    constexpr std::size_t MaxCallerBufferSize = 50;

    // 同时处理的 server 的消息最大数量
    constexpr std::size_t MaxServerBufferSize = 20;

    // 同时处理的 listener 的消息最大数量
    constexpr std::size_t MaxListenerBufferSize = 30;
}
