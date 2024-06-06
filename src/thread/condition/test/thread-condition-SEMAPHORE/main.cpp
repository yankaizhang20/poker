//
// Created by zyk on 24-6-6.
//

#include <thread>

#include <poker/system.h>
#include <poker/thread/condition.h>


int main()
{
    poker::thread::Semaphore semaphore(2);

    std::vector< std::thread > threads(10);

    for (size_t i = 0, size = threads.size(); i < size; ++i)
    {
        threads[ i ] = std::thread(
                [ &, i ]()
                {
                    if (not semaphore.Acquire())
                    {
                        std::cout << "fail " << i << std::endl;
                        return;
                    }

                    std::cout << "acquire " << i << std::endl;
                    poker::system::Sleep(poker::unit::Second(i + 1.0));
                    std::cout << "release " << i << std::endl;
                    semaphore.Release();

                    if (i == 2)
                    {
                        std::cout << "resize" << std::endl;
                        semaphore.SetMaxCount(3);
                    }
                });
    }

    poker::system::Sleep(poker::unit::Second(7));
    semaphore.FailAll();

    for (auto &i : threads)
        if (i.joinable())
            i.join();

    return 0;
}