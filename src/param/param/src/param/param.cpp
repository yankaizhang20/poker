//
// Created by zyk on 24-6-3.
//

#include "poker/param.h"


namespace poker::param
{
    void SetReader(Storage::Reader reader)
    {
        pattern::GetGlobalUnique< Storage >::Ref().SetReader(std::move(reader));
    }

    void SetWriter(Storage::Writer writer)
    {
        pattern::GetGlobalUnique< Storage >::Ref().SetWriter(std::move(writer));
    }

    void UnsetReader()
    {
        SetReader(nullptr);
    }

    void UnsetWriter()
    {
        SetWriter(nullptr);
    }
}   // namespace poker::param