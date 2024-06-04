//
// Created by zyk on 24-3-7.
//

#pragma once
#include <string>


namespace poker::databus
{
    struct Method
    {
        static std::string name()
        {
            return "default";
        }
    };

    struct XChannelType
    {
        std::string ip;
        std::size_t port = -1;
        std::string request_target;
    };
}   // namespace poker::databus