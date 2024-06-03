//
// Created by zyk on 24-6-3.
//

#pragma once

#include <string>

#include <yaml-cpp/yaml.h>

#include <poker/param.h>


namespace poker::param::backend
{
    class YamlEngine
    {
    public:
        void SetFileRoot(const std::string &root);

        [[nodiscard]]
        Storage::Reader GetReader() const;

        [[nodiscard]]
        Storage::Writer GetWriter() const;

    public:
        bool Read(const std::string &id, reflect::TypeView &parameter) const;

        bool Write(const std::string &id, const reflect::TypeView &parameter) const;

    private:
        static bool Load(const YAML::Node &yaml, reflect::TypeView &parameter);

        static YAML::Node Dump(const reflect::TypeView &parameter);

    private:
        static void Read(const YAML::Node &yaml, reflect::TypeView &parameter, bool &status);

        static void Read(const YAML::Node &yaml, reflect::TypeView::Value &value, bool &status);

        static void Read(const YAML::Node &yaml, reflect::TypeView::Enum &value, bool &status);

        static void Read(const YAML::Node &yaml, reflect::TypeView::Struct &struct_parameter, bool &status);

        static void Read(const YAML::Node &yaml, reflect::TypeView::Vector &vector_parameter, bool &status);

        static void Read(const YAML::Node &yaml, reflect::TypeView::List &list_parameter, bool &status);

        static void Read(const YAML::Node &yaml, reflect::TypeView::Map &dict_parameter, bool &status);

    private:
        static void Write(YAML::Node &yaml, const reflect::TypeView &parameter);

        static void Write(YAML::Node &yaml, const reflect::TypeView::Value &value);

        static void Write(YAML::Node &yaml, const reflect::TypeView::Enum &value);

        static void Write(YAML::Node &yaml, const reflect::TypeView::Struct &struct_parameter);

        static void Write(YAML::Node &yaml, const reflect::TypeView::Vector &vector_parameter);

        static void Write(YAML::Node &yaml, const reflect::TypeView::List &list_parameter);

        static void Write(YAML::Node &yaml, const reflect::TypeView::Map &dict_parameter);

    private:
        std::string GetFilePath(const std::string &id) const;

    private:
        // 参数文件根路径
        std::string root_;
    };
}   // namespace poker::param::backend