//
// Created by zyk on 24-5-9.
//

#include <poker/system/launch_center.h>

#include "poker/core.h"


namespace poker
{
    void init()
    {
        // 启动中心启动所有模块
        system::GetLaunchCenter().launch();
    }
}   // namespace poker