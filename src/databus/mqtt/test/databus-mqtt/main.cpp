//
// Created by zyk on 24-3-6.
//

#include <iostream>
#include <thread>

#include <poker/databus.h>
#include <poker/system.h>

#include "./channel.h"

#include <poker/databus/mqtt.h>


using namespace poker;

int main(int argc, char **argv)
{
    system::Init();

    // 等待 http 监听启动（仅为异步注册测试）
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // 配置通道数据
    databus_impl::channel_config< channel::MyServiceChannel > = {
            .ip = "localhost", .port = 65'534, .request_target = "/"};

    // 注册服务
    databus::Serve< channel::MyServiceChannel >(
            [](const int &, int &rep)
            {
                static int inner_num = 0;

                rep = inner_num++;

                return true;
            });

    // 调用服务
    std::optional< int > res;

    int seq = 0;

    while (seq < 100)
    {
        res = databus::Call< channel::MyServiceChannel >(1);

        if (res.has_value())
        {
            std::cout << "response is " << res.value() << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));

        seq++;
    }

    std::cout << "done" << std::endl;

    return 0;
}