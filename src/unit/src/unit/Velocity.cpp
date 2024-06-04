//
// Created by zyk on 2024-6-4 on 2021/8/13.
//

#include <poker/unit/Velocity.h>

namespace poker::unit::details
{
    template<>
    std::vector<double> Ratios<VelocityUnit>()
    {
        return {1.0, 3.6, 3.6 / 1.609344};
    }
}
