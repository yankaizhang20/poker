//
// Created by zyk on 24-3-8.
//

#pragma once
#include "./XChannelType.h"


namespace poker::databus
{
    template < class TRequest, class TResponse, class TTag = void >
    struct XServiceChannel
    {
        using Tag          = TTag;
        using RequestType  = TRequest;
        using ResponseType = TResponse;
        using ImplType     = XChannelType;
    };
}   // namespace poker::databus