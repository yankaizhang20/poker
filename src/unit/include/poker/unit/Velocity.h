//
// Created by zyk on 2024-6-4
//

#pragma once

#include "./details/Unit.h"


namespace poker::unit
{
    enum class VelocityUnit
    {
        Undefined = -1,
        M_PER_SEC,
        KM_PER_H,
        MPH,
        Default = 0
    };

    using Velocity  = details::UnitSystem<VelocityUnit>;

    using MeterPerSecond   = details::Unit<VelocityUnit, VelocityUnit::M_PER_SEC>;
    using KilometerPerHour = details::Unit<VelocityUnit, VelocityUnit::KM_PER_H>;
    using MilePerHour      = details::Unit<VelocityUnit, VelocityUnit::MPH>;

    poker_concept(VelocityClass, class T, (std::is_base_of_v<Velocity, T>));
}

namespace pokeru
{
    using mps = poker::unit::MeterPerSecond;
    using kmh = poker::unit::KilometerPerHour;
    using kph = kmh;
    using mph = poker::unit::MilePerHour;
}
