//
// Created by zyk on 24-6-5.
//

#include <chrono>

#include "poker/chrono.h"


namespace poker::chrono
{
    unit::Time now()
    {
        return {poker::unit::TimeUnit::Second, sec()};
    }

    double sec()
    {
        return double(us()) * 1e-6;
    }

    long long ms()
    {
        return std::chrono::duration_cast< std::chrono::milliseconds >(
                       std::chrono::system_clock::now().time_since_epoch())
                .count();
    }

    long long us()
    {
        return std::chrono::duration_cast< std::chrono::microseconds >(
                       std::chrono::system_clock::now().time_since_epoch())
                .count();
    }

    long long ns()
    {
        return std::chrono::system_clock::now().time_since_epoch().count();
    }

    std::string date()
    {
        auto  t  = std::time(nullptr);
        auto *lt = std::localtime(&t);

        char buffer[ 128 ];

        int n = std::sprintf(buffer, "%d-%02d-%02d_%02d:%02d:%02d", lt->tm_year + 1'900, lt->tm_mon + 1, lt->tm_mday,
                             lt->tm_hour, lt->tm_min, lt->tm_sec);

        return std::string(buffer, n);
    }
}   // namespace poker::chrono
