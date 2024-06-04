//
// Created by zyk on 2024-6-4
//

#pragma once

#include "./details/Unit.h"


namespace poker::unit
{
    enum class AngleUnit
    {
        Undefined = -1,
        Degree,
        Radian,
        Default = 0
    };

    using Angle  = details::UnitSystem<AngleUnit>;

    using Degree = details::Unit<AngleUnit, AngleUnit::Degree>;
    using Radian = details::Unit<AngleUnit, AngleUnit::Radian>;

    poker_concept(AngleClass, class T, (std::is_base_of_v<Angle, T>));
}

namespace pokeru
{
    using rad = poker::unit::Radian;
    using deg = poker::unit::Degree;
}
