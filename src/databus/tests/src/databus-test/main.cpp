//
// Created by zyk on 24-6-6.
//

#include <poker/databus.h>
#include <poker/serialization.h>
#include <poker/system.h>

#include "channel.h"


void TestTopic()
{
    poker::databus::Listen< channel::MyTopicChannel >(
            [](bool x) { std::cout << "M received " << (x ? "yes" : "no") << std::endl; });

    poker::databus::Send< channel::MyTopicChannel >(true);
    poker::databus::Send< channel::MyTopicChannel >(false);

    poker::system::Sleep(pokeru::sec(1.0));

    poker::databus::Offline< channel::MyTopicChannel >();
    poker::databus::Send< channel::MyTopicChannel >(true);

    poker::system::Sleep(pokeru::sec(1.0));
}

void TestService()
{
    poker::databus::Serve< channel::MyServiceChannel >(
            [](const int &req, bool &rep) -> bool
            {
                std::cout << "S serve " << req << std::endl;
                rep = req % 2;
                return true;
            });

    std::optional< bool > rep = poker::databus::Call< channel::MyServiceChannel >(3);

    if (rep.has_value())
        std::cout << "S response is " << (rep.value() ? "yes" : "no") << std::endl;

    poker::databus::Offline< channel::MyServiceChannel >();

    if (not poker::databus::Call< channel::MyServiceChannel >(3).has_value())
    {
        std::cout << "S is offline" << std::endl;
    }
    else
    {
        std::cout << "S is still online" << std::endl;
    }
}

int main(int argc, char **argv)
{
    poker::system::Init();

    TestTopic();
    TestService();

    poker::system::Yield();

    return 0;
}