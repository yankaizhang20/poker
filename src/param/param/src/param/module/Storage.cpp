//
// Created by zyk on 24-6-3.
//

#include "poker/param/module/Storage.h"


namespace poker::param
{
    void Storage::SetReader(Storage::Reader reader)
    {
        _reader = std::move(reader);
    }

    void Storage::SetWriter(Storage::Writer writer)
    {
        _writer = std::move(writer);
    }

    bool Storage::Read(const std::string &id, reflect::TypeView &parameter)
    {
        if (_reader)
            return _reader(id, parameter);
        else
            return false;
    }

    bool Storage::Write(const std::string &id, const reflect::TypeView &parameter)
    {
        if (_writer)
            return _writer(id, parameter);
        else
            return false;
    }
}   // namespace poker::param