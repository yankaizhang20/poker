//
// Created by zyk on 24-3-7.
//

#pragma once

#include <poker/system.h>

#include "./Center.h"


namespace poker::databus::http::auto_launch
{
    class CenterModule : public system::Modular< CenterModule, Center >
    {
    protected:
        void ModuleStart() override;

        void ModuleShutdown() override;

        std::size_t GetLaunchPriority() override;
    };

    POKER_NO_OPTIMIZED(CenterModule)
}   // namespace poker::databus::http::auto_launch
