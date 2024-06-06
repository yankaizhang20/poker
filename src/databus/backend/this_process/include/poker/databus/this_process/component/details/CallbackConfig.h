//
// Created by zyk on 2-24-6-6.
//

#pragma once


namespace poker::databus::details
{
    struct CallbackConfig
    {
        // 在回调进行时，是否丢弃新提交的数据，从而避免数据排队。
        bool enable_discard_calls = false;
    };
}   // namespace poker::databus::details
