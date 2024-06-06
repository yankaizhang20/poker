//
// Created by zyk on 2024-6-5
//

#pragma once

#include <any>
#include <functional>

#include <poker/macro.h>

#include <poker/databus.h>


namespace poker::databus::this_process
{
    class ServiceWrapper
    {
        template < class TRequest, class TResponse >
        using Core = Server< TRequest, TResponse >;

    public:
        template < class TRequest, class TResponse >
        void SetService(const Core< TRequest, TResponse > &server)
        {
            if (_core.has_value())
                poker_never("set service twice on the same channel !");

            if (server == nullptr)
                return;

            _core = server;
        }

        void UnsetService()
        {
            _core.reset();
        }

        template < class TRequest, class TResponse >
        bool CallService(const TRequest &request, TResponse &response)
        {
            if (not _core.has_value())
                return false;

            try
            {
                return std::any_cast< Core< TRequest, TResponse > & >(_core)(request, response);
            }
            catch (std::exception &ec)
            {
                std::cout << ec.what() << std::endl;
                return false;
            }
        }

    private:
        // 存放一个具体类型的服务函数
        std::any _core;
    };
}   // namespace poker::databus::this_process