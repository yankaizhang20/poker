//
// Created by zyk on 24-6-6.
//

#include <iostream>
#include <poker/system.h>
#include <poker/thread/Pool.h>


int main()
{
    poker::thread::Pool pool(1);

    pool.Add(
            []()
            {
                printf("task 1\n");
                poker::system::Sleep(pokeru::sec(3));
            });

    auto task2 = pool.Add(
            []() -> int
            {
                printf("task 2\n");

                return 3;
            });

    std::optional< int > task2_result = task2->WaitResult();

    if (task2_result.has_value())
        std::cout << "get task 2 result: " << task2_result.value() << std::endl;
    else
        std::cout << "task 2 fails";

    for (int i = 3; i < 20; ++i)
    {
        pool.Add(
                [ i ]()
                {
                    printf("task %d\n", i);
                    poker::system::Sleep(pokeru::sec(2));
                });
    }

    poker::system::Sleep(pokeru::sec(5));
    pool.Resize(3);
    pool.Wait(8);

    return 0;
}