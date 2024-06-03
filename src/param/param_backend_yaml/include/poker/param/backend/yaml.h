//
// Created by zyk on 24-6-3.
//

#pragma once

#include <string>

#include <poker/param.h>


namespace poker::param::backend
{
    class YamlEngine
    {
    public:
        void SetFileRoot(const std::string &root);

        Storage::Reader GetReader() const;

        Storage::Writer GetWriter() const;

    public:
        bool Read(const std::string &id, reflect::TypeView &parameter) const;

        bool Write(const std::string &id, reflect::TypeView &parameter) const;

    private:
        static bool Load(const YAML::Node &yaml, ParameterView &parameter);

        static YAML::Node Dump(const ParameterView &parameter);

    private:
        // 参数文件根路径
        std::string root_;
    };
}   // namespace poker::param::backend