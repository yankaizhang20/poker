//
// Created by zyk on 24-6-4.
//

#pragma once

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

}   // namespace poker::system