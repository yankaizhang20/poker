//
// Created by zyk on 24-4-10.
//

#pragma once
#include "./entity.h"
#include "./interface.h"

int main()
{
    // 各种业务实体 object
    Derive1< BaseA > d1_a;
    Derive1< BaseB > d1_b;

    Derive2< BaseA > d2_a;
    Derive2< BaseB > d2_b;

    SpecialDerive< BaseA > d_spec_a;
    SpecialDerive< BaseB > d_spec_b;

    // 处理正常业务实体的基类部分数据
    Helper< Derive1< BaseA > >::func(d1_a);
    Helper< Derive1< BaseB > >::func(d1_b);

    Helper< Derive2< BaseA > >::func(d2_a);
    Helper< Derive2< BaseB > >::func(d2_b);

    // 处理特殊业务实体，note: SpecialDerive< BaseA > 会被处理为 BaseA
    Helper< SpecialDerive< BaseA > >::func(d_spec_a);
    Helper< SpecialDerive< BaseB > >::func(d_spec_b);

    return 0;
}
