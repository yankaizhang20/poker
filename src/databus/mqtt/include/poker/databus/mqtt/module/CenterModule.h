//
// Created by zyk on 2024-6-4 on 22-7-15.
//

#pragma once

#include <poker/base/Modular.h>

#include "./Center.h"


namespace poker::databus::mqtt::auto_launch
{
    class CenterModule :
        public Modular<CenterModule, Center>
    {
    protected:
        bool ModuleStart() override;

        void ModuleShutdown() override;

        unsigned int GetLaunchPriority() override;
    };

    NOX_NOT_OPTIMIZE_OUT(CenterModule)
}
