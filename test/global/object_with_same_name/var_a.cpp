//
// Created by zyk on 24-4-19.
//

#include "./Object.h"
#include "./var_a_api.h"

namespace aspace
{
    DEF_OBJECT(a);

    void APIFunc()
    {
        std::cout << "APIFunc run from var_a" << std::endl;
    }
}   // namespace aspace

aspace::Object object;