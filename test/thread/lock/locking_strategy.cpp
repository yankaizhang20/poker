//
// Created by zyk on 24-3-10.
//
#include <mutex>
#include <thread>

std::mutex lock1;
std::mutex lock2;

int main()
{
    std::thread child(
            [ & ]()
            {
                {
                    std::lock(lock1, lock2);
                    std::lock_guard< std::mutex > guard1(lock1, std::adopt_lock);
                    std::lock_guard< std::mutex > guard2(lock2, std::adopt_lock);
                }
            });
    child.join();
}