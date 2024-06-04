//
// Created by zyk on 24-5-9.
//

#include "poker/system/LaunchCenter.h"


namespace poker::system
{
    LaunchCenter &GetLaunchCenter()
    {
        return pattern::GetGlobalUnique< LaunchCenter >::Ref();
    }

    void LaunchCenter::add(IComponent *component)
    {
        comps_.emplace(component->GetLaunchPriority(), component);
    }

    void LaunchCenter::launch()
    {
        for (auto &[ priority, p_component ] : comps_)
        {
            p_component->ModuleStart();
        }
    }

    void LaunchCenter::shutdown()
    {
        for (auto &[ priority, p_component ] : comps_)
        {
            p_component->ModuleShutdown();
        }
    }
}   // namespace poker::system