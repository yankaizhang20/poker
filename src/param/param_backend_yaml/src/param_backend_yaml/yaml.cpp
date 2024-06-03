//
// Created by zyk on 24-6-3.
//

#include <fstream>

#include <poker/reflect.h>

#include "poker/param/backend/yaml.h"


namespace poker::param::backend
{
    namespace details
    {
#define DECLARE_READ_FUNC(_type_)                                                                          \
    static void Read(const YAML::Node &yaml, reflect::trait::value_t< reflect::ValueType::_type_ > &value) \
    {                                                                                                      \
        value = yaml.as< std::remove_reference_t< decltype(value) > >();                                   \
    }

        POKER_VALUE_REFLECT_TYPE(DECLARE_READ_FUNC)
#undef DECLARE_READ_FUNC
    }   // namespace details

    void YamlEngine::SetFileRoot(const std::string &root)
    {
        root_ = root;
    }

    Storage::Reader YamlEngine::GetReader() const
    {
        return [ this ](const std::string &id, reflect::TypeView &parameter) { return Read(id, parameter); };
    }

    Storage::Writer YamlEngine::GetWriter() const
    {
        return [ this ](const std::string &id, const reflect::TypeView &parameter) { return Write(id, parameter); };
    }

    bool YamlEngine::Read(const std::string &id, reflect::TypeView &parameter) const
    try
    {
        YAML::Node yaml = YAML::LoadFile(GetFilePath(id));

        return Load(yaml, parameter);
    }
    catch (...)
    {
        return false;
    }

    bool YamlEngine::Write(const std::string &id, const reflect::TypeView &parameter) const
    {
        // step 1: 打开参数文件，若无法打开，则直接返回 false
        std::ofstream file(GetFilePath(id));

        if (not file.is_open())
            return false;

        // step 2: 将参数的内容写入到 YAML 对象中
        YAML::Node yaml = Dump(parameter);

        // step 3: 将 yaml 序列化到文件中
        file << YAML::Dump(yaml);

        return not file.bad();
    }

    bool YamlEngine::Load(const YAML::Node &yaml, reflect::TypeView &parameter)
    try
    {
        bool status = true;
        Read(yaml, parameter, status);
        return status;
    }
    catch (...)
    {
        return false;
    }

    YAML::Node YamlEngine::Dump(const reflect::TypeView &parameter)
    {
        YAML::Node yaml;
        Write(yaml, parameter);
        return yaml;
    }

    void YamlEngine::Read(const YAML::Node &yaml, reflect::TypeView &parameter, bool &status)
    {
        switch (parameter.GetType())
        {
            case reflect::ReflectType::Undefined:
                break;

#define READ_CASE(Type)                           \
    case reflect::ReflectType::Type:              \
        Read(yaml, parameter.As##Type(), status); \
        break;

                POKER_ALL_REFLECT_TYPE(READ_CASE)
#undef READ_CASE

            default:
                poker_no_impl();
        }
    }

    void YamlEngine::Read(const YAML::Node &yaml, reflect::TypeView::Value &value, bool &status)
    try
    {
        switch (value.GetType())
        {
#define READ_CASE(_type_)                        \
    case reflect::ValueType::_type_:             \
        details::Read(yaml, value.As##_type_()); \
        break;

            POKER_VALUE_REFLECT_TYPE(READ_CASE)

#undef READ_CASE

            default:
                poker_no_impl();
        }
    }
    catch (...)
    {
        status = false;
    }

    void YamlEngine::Read(const YAML::Node &yaml, reflect::TypeView::Enum &value, bool &status)
    {
    }

}   // namespace poker::param::backend