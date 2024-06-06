//
// Created by zyk on 2024-6-4 on 22-7-15.
//

#pragma once

#include <poker/system.h>

#include "./Center.h"


namespace poker::databus::mqtt::auto_launch
{
    class CenterModule : public system::Modular< CenterModule, Center >
    {
    protected:
        void ModuleStart() override;

        void ModuleShutdown() override;

        std::size_t GetLaunchPriority() override;
    };

    POKER_NO_OPTIMIZED(CenterModule)
}   // namespace poker::databus::mqtt::auto_launch
