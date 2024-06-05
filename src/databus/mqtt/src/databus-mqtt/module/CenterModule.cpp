//
// Created by zyk on 2024-6-4 on 22-7-15.
//

#include <poker/databus/mqtt/module/CenterModule.h>


namespace poker::databus::mqtt::auto_launch
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
}   // namespace poker::databus::mqtt::auto_launch
