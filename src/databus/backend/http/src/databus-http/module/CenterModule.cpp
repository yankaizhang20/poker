//
// Created by zyk on 24-3-7.
//

#include "poker/databus/http/module/CenterModule.h"


namespace poker::databus::http::auto_launch
{
    void CenterModule::ModuleStart()
    {
        unique_core.Setup();
    }

    void CenterModule::ModuleShutdown()
    {
        unique_core.Shutdown();
    }

    std::size_t CenterModule::GetLaunchPriority()
    {
        return 30;
    }
}   // namespace poker::databus::http::auto_launch