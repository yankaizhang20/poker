//
// Created by zyk on 2024-6-4 on 2021/8/13.
//

#include <poker/unit/Time.h>


namespace poker::unit::details
{
    template<>
    std::vector<double> Ratios<TimeUnit>()
    {
        return {1.0, 1.0/60.0, 1.0/3600.0, 1.0/86400.0, 1.0/604800.0, 1e3, 1e6};
    }
}
