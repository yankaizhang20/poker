//
// Created by zyk on 24-3-7.
//
#include "poker/databus/http/module/Center.h"

#include "poker/databus/http/component/CallerCenter.h"
#include "poker/databus/http/component/ServerCenter.h"


namespace poker::databus::http
{
    static constexpr const char *param_key = "poker.databus.http.Center";

    bool Center::Read(Param &param)
    {
        return poker::param::Read(param_key, param);
    }

    bool Center::Write(const Param &param)
    {
        return poker::param::Write(param_key, param);
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
}   // namespace poker::databus::http