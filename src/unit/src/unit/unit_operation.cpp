//
// Created by zyk on 2024-6-4 on 22-6-8.
//

#include <poker/unit.h>


namespace poker::unit
{
    Time operator/(const Distance & distance, const Velocity & velocity)
    {
        return pokeru::sec( distance.Get<pokeru::m>() / velocity.Get<pokeru::mps>() );
    }
}
