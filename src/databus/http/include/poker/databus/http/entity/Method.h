//
// Created by zyk on 24-3-14.
//

#pragma once
#include <string>
#include <ares/databus/entity/XChannelType.h>


namespace ares::databus::http
{
    struct Get : public Method
    {
        static std::string name()
        {
            return "Get";
        }
    };

    struct Post : public Method
    {
        static std::string name()
        {
            return "Post";
        }
    };

    struct Patch : public Method
    {
        static std::string name()
        {
            return "Patch";
        }
    };

    struct Delete : public Method
    {
        static std::string name()
        {
            return "Delete";
        }
    };

    struct Options : public Method
    {
        static std::string name()
        {
            return "Options";
        }
    };
}   // namespace ares::databus::http