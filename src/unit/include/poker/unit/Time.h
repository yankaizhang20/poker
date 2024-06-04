//
// Created by zyk on 2024-6-4
//

#pragma once

#include <chrono>
#include "./details/Unit.h"


namespace poker::unit
{
    enum class TimeUnit
    {
        Undefined = -1,
        Second,
        Minute,
        Hour,
        Day,
        Week,
        MilliSecond,
        MicroSecond,
        Default = 0
    };

    using Time        = details::UnitSystem<TimeUnit>;

    using Second      = details::Unit<TimeUnit, TimeUnit::Second>;
    using Minute      = details::Unit<TimeUnit, TimeUnit::Minute>;
    using Hour        = details::Unit<TimeUnit, TimeUnit::Hour>;
    using Day         = details::Unit<TimeUnit, TimeUnit::Day>;
    using Week        = details::Unit<TimeUnit, TimeUnit::Week>;
    using MilliSecond = details::Unit<TimeUnit, TimeUnit::MilliSecond>;
    using MicroSecond = details::Unit<TimeUnit, TimeUnit::MicroSecond>;

    poker_concept(TimeClass, class T, (std::is_base_of_v<Time, T>));
}

namespace pokeru
{
    using sec  = poker::unit::Second;
    using min  = poker::unit::Minute;
    using hour = poker::unit::Hour;
    using day  = poker::unit::Day;
    using week = poker::unit::Week;
    using ms   = poker::unit::MilliSecond;
    using us   = poker::unit::MicroSecond;
}

namespace poker::utils
{
    /**
     * @brief 获取给定一段时间之后的系统时间点，以 std 的格式返回
     * @param time_interval 距离调用瞬间的时间间隔
     * @return 当前系统时间 + 时间间隔后得到的系统时间点
     */
    inline auto TimePointFromNow(const poker::unit::Time & time_interval)
    {
        auto us = int64_t( time_interval.Get<pokeru::us>() );
        return std::chrono::system_clock::now() + std::chrono::microseconds(us);
    }
}
