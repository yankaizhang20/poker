//
// Created by zyk on 24-3-7.
//
#include "ares/databus/http/module/Center.h"

#include "ares/databus/http/component/CallerCenter.h"
#include "ares/databus/http/component/ServerCenter.h"


namespace ares::databus::http
{
    static constexpr const char *param_key = "ares.databus.http.Center";

    bool Center::Read(Param &param)
    {
        return nox::param::Read(param_key, param);
    }

    bool Center::Write(const Param &param)
    {
        return nox::param::Write(param_key, param);
    }

    void Center::Setup()
    {
        // 读取参数
        Read(_param);

        // 注册参数
        _server_center->SetParam(_param.server_param);
        _caller_center->SetParam(_param.caller_param);

        // 初始化各组件
        _server_center->Setup();
        _caller_center->Setup();
    }

    void Center::Shutdown()
    {
        _server_center->Shutdown();
        _caller_center->Shutdown();
    }
}   // namespace ares::databus::http