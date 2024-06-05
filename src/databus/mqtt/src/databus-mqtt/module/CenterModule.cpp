//
// Created by zyk on 2024-6-4 on 22-7-15.
//

#include <poker/databus/mqtt/module/CenterModule.h>


namespace poker::databus::mqtt::auto_launch
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
}
