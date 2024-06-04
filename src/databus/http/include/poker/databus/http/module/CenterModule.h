//
// Created by zyk on 24-3-7.
//

#pragma once

#include <ares/base/Modular.h>

#include "./Center.h"


namespace ares::databus::http::auto_launch
{
    class CenterModule : public Modular< CenterModule, Center >
    {
    protected:
        bool ModuleStart() override;

        void ModuleShutdown() override;

        unsigned int GetLaunchPriority() override;
    };

    NOX_NOT_OPTIMIZE_OUT(CenterModule)
}   // namespace ares::databus::http::auto_launch
