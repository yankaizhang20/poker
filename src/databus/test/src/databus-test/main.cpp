//
// Created by zyk on 24-6-6.
//

#include <poker/databus.h>
#include <poker/system.h>

#include "./channel.h"


int main()
{
    std::mutex m;

    poker::databus::aux< channel::MyTopicChannel > += poker::databus::Listen< channel::MyTopicChannel >(
            [ & ](test::MyType1 msg)
            {
                std::unique_lock lock(m);
                std::cout << "receive message 1: " << msg.a << ", " << msg.b << ", " << msg.c << std::endl;
            });

    poker::databus::Listen< channel::MyTopicChannel >(
            [ & ](test::MyType1 msg)
            {
                std::unique_lock lock(m);
                std::cout << "receive message 2: " << msg.a << ", " << msg.b << ", " << msg.c << std::endl;
            });

    test::MyType1 msg;

    for (int i = 0; i < 10; ++i)
    {
        poker::databus::Send< channel::MyTopicChannel >(msg);

        ++msg.a;
        msg.b  = not msg.b;
        msg.c -= 2.0;

        if (i == 5)
        {
            // 先等之前的线程打印完毕，再关闭通道。否则看不到效果。
            poker::system::Sleep(pokeru::sec(1));

            // 关闭指定通道
            poker::databus::aux< channel::MyTopicChannel >.Offline();
        }
    }

    poker::system::Sleep(pokeru::sec(1));

    auto TestRequest = [](bool b)
    {
        test::MyType1 request;
        request.b = b;

        std::optional< test::MyType2 > response = poker::databus::Call< channel::MyServiceChannel >(request);

        if (response.has_value())
            std::cout << "call success with size " << response.value().data.size() << std::endl;
        else
            std::cout << "call fail" << std::endl;
    };

    TestRequest(true);

    poker::databus::Serve< channel::MyServiceChannel >(
            [](const test::MyType1 &request, test::MyType2 &response) -> bool
            {
                if (not request.b)
                    return false;

                response.data.resize(request.a, request.c);

                return true;
            });

    TestRequest(true);
    TestRequest(false);

    poker::databus::Offline< channel::MyServiceChannel >();

    TestRequest(true);

    return 0;
}