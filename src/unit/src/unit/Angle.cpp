//
// Created by zyk on 2024-6-4 on 2021/8/13.
//

#include <poker/unit/Angle.h>
#include <cmath>


namespace poker::unit::details
{
    template<>
    std::vector<double> Ratios<AngleUnit>()
    {
        return {1.0, M_PI / 180.0};
    }
}
