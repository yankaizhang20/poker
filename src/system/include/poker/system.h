//
// Created by zyk on 24-6-4.
//

#pragma once

#include <poker/unit.h>

#include "./system/Modular.h"


namespace poker::system
{
    /**
     * @brief 初始化 poker
     */
    void Init();

    /**
     * @brief 关闭 poker
     */
    void Shutdown();

    /**
     * @brief 阻塞等待进程被要求退出
     */
    void Yield();

    /**
     * @return 本进程是否准备退出，可用于无限循环中的条件判断
     */
    bool Ok();

    /**
     * @brief 休眠指定时间
     */
    void Sleep(const unit::Time &time);

}   // namespace poker::system