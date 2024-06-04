//
// Created by zyk on 2024-6-4
//

#pragma once


#include "./details/Unit.h"


namespace poker::unit
{
    enum class DistanceUnit
    {
        Undefined = -1,
        Meter,
        Kilometer,
        Mile,
        Default = 0
    };

    using Distance  = details::UnitSystem<DistanceUnit>;

    using Meter     = details::Unit<DistanceUnit, DistanceUnit::Meter>;
    using Kilometer = details::Unit<DistanceUnit, DistanceUnit::Kilometer>;
    using Mile      = details::Unit<DistanceUnit, DistanceUnit::Mile>;

    poker_concept(DistanceClass, class T, (std::is_base_of_v<Distance, T>));
}

namespace pokeru
{
    using m  = poker::unit::Meter;
    using km = poker::unit::Kilometer;
    using mi = poker::unit::Mile;
}
