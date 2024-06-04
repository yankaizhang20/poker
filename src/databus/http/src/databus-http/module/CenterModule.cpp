//
// Created by zyk on 24-3-7.
//

#include "ares/databus/http/module/CenterModule.h"


namespace ares::databus::http::auto_launch
{
    bool CenterModule::ModuleStart()
    {
        unique_core.Setup();
        return true;
    }

    void CenterModule::ModuleShutdown()
    {
        unique_core.Shutdown();
    }

    unsigned int CenterModule::GetLaunchPriority()
    {
        return 30;
    }
}   // namespace ares::databus::http::auto_launch