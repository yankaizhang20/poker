//
// Created by zyk on 24-5-9.
//

#pragma once

#include <poker/AutoComponent.h>


namespace poker
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

}   // namespace poker