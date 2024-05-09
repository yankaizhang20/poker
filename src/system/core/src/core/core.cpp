//
// Created by zyk on 24-5-9.
//

#include <csignal>
#include <future>
#include <iostream>

#include <poker/system/launch_center.h>

#include "poker/core.h"


namespace poker
{
    namespace
    {
        // 用于通知进程结束
        std::promise< void > wait_closed;
        std::future          is_closed_required = wait_closed.get_future();

        // 信号函数
        void SignalHandler(int signal)
        {
            switch (signal)
            {
                case SIGINT:
                case SIGTERM:
                {
                    std::cout << " ==== process exit! ====" << std::endl;
                    wait_closed.set_value();
                }
                break;
            }
        }
    }   // namespace

    void Init()
    {
        // 处理终止信号
        std::signal(SIGTERM, SignalHandler);
        std::signal(SIGINT, SignalHandler);

        // 启动中心启动所有模块
        system::GetLaunchCenter().launch();
    }

    void Shutdown()
    {
        // 启动中心关闭所有模块
        system::GetLaunchCenter().shutdown();
    }

    void Yield()
    {
        is_closed_required.wait();

        Shutdown();
    }
}   // namespace poker