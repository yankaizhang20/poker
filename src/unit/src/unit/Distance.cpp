//
// Created by zyk on 2024-6-4 on 2021/8/13.
//

#include <poker/unit/Distance.h>


namespace poker::unit::details
{
    template<>
    std::vector<double> Ratios<DistanceUnit>()
    {
        return {1.0, 0.001, 0.00062137};
    }
}
