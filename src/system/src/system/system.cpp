//
// Created by zyk on 24-5-9.
//

#include <csignal>
#include <future>
#include <iostream>

#include "poker/system.h"
#include "poker/system/LaunchCenter.h"


namespace poker::system
{
    namespace
    {
        // 控制进程的结束与否
        std::atomic_bool is_process_stopped = false;

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
        // 标识进程退出
        is_process_stopped = true;

        // 启动中心关闭所有模块
        system::GetLaunchCenter().shutdown();
    }

    void Yield()
    {
        is_closed_required.wait();

        Shutdown();
    }

    bool Ok()
    {
        return is_process_stopped;
    }

    void Sleep(const unit::Time &time)
    {
        auto us = long(time.Get< pokeru::us >());
        std::this_thread::sleep_for(std::chrono::microseconds(us));
    }
}   // namespace poker::system