//
// Created by zyk on 2024-6-4
//

#pragma once

#include "./details/Unit.h"


namespace poker::unit
{
    enum class WeightUnit
    {
        Undefined = -1,
        KG,
        G,
        Default = 0
    };

    using Weight   = details::UnitSystem<WeightUnit>;

    using Kilogram = details::Unit<WeightUnit, WeightUnit::KG>;
    using Gram     = details::Unit<WeightUnit, WeightUnit::G>;

    poker_concept(WeightClass, class T, (std::is_base_of_v<Weight, T>));
}

namespace pokeru
{
    using kg = poker::unit::Kilogram;
    using g  = poker::unit::Gram;
}
