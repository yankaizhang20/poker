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

        POKER_BASIC_VALUE_REFLECT_TYPE(DECLARE_READ_FUNC)
#undef DECLARE_READ_FUNC


        static void CheckTag(const YAML::Node &yaml, reflect::ValueType type)
        {
            if (yaml.Tag() != GetFullValueTypeName(type))
                throw std::runtime_error("wrong yaml tag");
        }

#define BEGIN_READ_UNIT_SYSTEM(Type)                            \
    static void Read(const YAML::Node &yaml, unit::Type &value) \
    {                                                           \
        CheckTag(yaml, reflect::ValueType::Type);               \
                                                                \
        std::string unit_name;                                  \
        double      unit_value;                                 \
                                                                \
        Read(yaml[ "unit" ], unit_name);                        \
        Read(yaml[ "value" ], unit_value);

#define READ_UNIT(unit)                        \
    if (unit_name == #unit)                    \
    {                                          \
        value.Set< pokeru::unit >(unit_value); \
        return;                                \
    }

#define END_READ_UNIT_SYSTEM(Type)                                                                     \
    throw std::runtime_error("wrong unit name for " + GetFullValueTypeName(reflect::ValueType::Type)); \
    }

#define READ_UNIT_SYSTEM(Type, ...) \
    BEGIN_READ_UNIT_SYSTEM(Type)    \
    __VA_ARGS__                     \
    END_READ_UNIT_SYSTEM(Type)

        READ_UNIT_SYSTEM(Angle, POKER_INVOKE(READ_UNIT, deg, rad))
        READ_UNIT_SYSTEM(Distance, POKER_INVOKE(READ_UNIT, m, km, mi))
        READ_UNIT_SYSTEM(Time, POKER_INVOKE(READ_UNIT, sec, min, hour, day, week, ms, us))
        READ_UNIT_SYSTEM(Velocity, POKER_INVOKE(READ_UNIT, mps, kmh, kph, mph))
        READ_UNIT_SYSTEM(Weight, POKER_INVOKE(READ_UNIT, kg, g))

#undef BEGIN_READ_UNIT_SYSTEM
#undef READ_UNIT
#undef END_READ_UNIT_SYSTEM
#undef READ_UNIT_SYSTEM

#define DECLARE_WRITE_FUNC(_type_)                                                                          \
    static void Write(YAML::Node &yaml, const reflect::trait::value_t< reflect::ValueType::_type_ > &value) \
    {                                                                                                       \
        yaml = value;                                                                                       \
    }

        POKER_BASIC_VALUE_REFLECT_TYPE(DECLARE_WRITE_FUNC)
#undef DECLARE_WRITE_FUNC

#define WRITE_UNIT(_Type_, _default_unit_)                             \
    static void Write(YAML::Node &yaml, const unit::_Type_ &value)     \
    {                                                                  \
        yaml.SetTag(GetFullValueTypeName(reflect::ValueType::_Type_)); \
        yaml[ "value" ] = value.Get< pokeru::_default_unit_ >();       \
        yaml[ "unit" ]  = #_default_unit_;                             \
    }

        WRITE_UNIT(Angle, deg)
        WRITE_UNIT(Distance, m)
        WRITE_UNIT(Time, sec)
        WRITE_UNIT(Velocity, kmh)
        WRITE_UNIT(Weight, kg)
#undef WRITE_UNIT
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
    try
    {
        if (yaml.Tag() != "enum")
            throw std::runtime_error("wrong yaml tag");

        // 尝试使用 enum name 进行值设置，若设置失败，再考虑直接使用
        // enum number 进行设置
        if (YAML::Node node = yaml[ "name" ]; node.IsScalar())
        {
            if (value.TrySetByName(node.as< std::string >()))
                return;
        }

        value.SetNumber(yaml[ "number" ].as< reflect::EnumNumber >());
    }
    catch (...)
    {
        status = false;
    }

    void YamlEngine::Read(const YAML::Node &yaml, reflect::TypeView::Struct &struct_parameter, bool &status)
    try
    {
        for (auto &it : struct_parameter)
        {
            Read(yaml[ it.first ], it.second, status);
        }
    }
    catch (...)
    {
        status = false;
    }

    void YamlEngine::Read(const YAML::Node &yaml, reflect::TypeView::Vector &vector_parameter, bool &status)
    {
        try
        {
            vector_parameter.ClearAndReserve(yaml.size());

            for (std::size_t i = 0, end = yaml.size(); i < end; ++i)
            {
                Read(yaml[ i ], vector_parameter.Add(), status);
            }
        }
        catch (...)
        {
            status = false;
        }
    }

    void YamlEngine::Read(const YAML::Node &yaml, reflect::TypeView::List &list_parameter, bool &status)
    try
    {
        list_parameter.Clear();

        for (std::size_t i = 0, end = yaml.size(); i < end; ++i)
        {
            Read(yaml[ i ], list_parameter.Add(), status);
        }
    }
    catch (...)
    {
        status = false;
    }

    void YamlEngine::Read(const YAML::Node &yaml, reflect::TypeView::Map &dict_parameter, bool &status)
    try
    {
        dict_parameter.Clear();

        for (auto &it : yaml)
        {
            Read(it.second, dict_parameter.Add(it.first.Scalar()), status);
        }
    }
    catch (...)
    {
        status = false;
    }

    void YamlEngine::Write(YAML::Node &yaml, const reflect::TypeView &parameter)
    {
        switch (parameter.GetType())
        {
            case reflect::ReflectType::Undefined:
                break;

#define WRITE_CASE(Type)                   \
    case reflect::ReflectType::Type:       \
        Write(yaml, parameter.As##Type()); \
        break;

                POKER_ALL_REFLECT_TYPE(WRITE_CASE)
#undef WRITE_CASE

            default:
                poker_no_impl();
        }
    }

    void YamlEngine::Write(YAML::Node &yaml, const reflect::TypeView::Value &value)
    {
        switch (value.GetType())
        {
#define WRITE_CASE(_type_)                        \
    case reflect::ValueType::_type_:              \
        details::Write(yaml, value.As##_type_()); \
        break;
            POKER_VALUE_REFLECT_TYPE(WRITE_CASE)
#undef WRITE_CASE

            default:
                poker_no_impl();
        }
    }

    void YamlEngine::Write(YAML::Node &yaml, const reflect::TypeView::Enum &value)
    {
        yaml.SetTag("enum");

        const std::vector< std::string > &names = value.GetNames();

        if (names.empty())
            yaml[ "number" ] = value.GetNumber();
        else
            yaml[ "name" ] = names.front();
    }

    void YamlEngine::Write(YAML::Node &yaml, const reflect::TypeView::Struct &struct_parameter)
    {
        for (auto &it : struct_parameter)
        {
            YAML::Node sub_yaml = yaml[ it.first ];
            Write(sub_yaml, it.second);
        }
    }

    void YamlEngine::Write(YAML::Node &yaml, const reflect::TypeView::Vector &vector_parameter)
    {
        for (auto &i : vector_parameter)
        {
            YAML::Node sub_yaml = yaml[ yaml.size() ];
            Write(sub_yaml, i);
        }
    }

    void YamlEngine::Write(YAML::Node &yaml, const reflect::TypeView::List &list_parameter)
    {
        for (auto &i : list_parameter)
        {
            YAML::Node sub_yaml = yaml[ yaml.size() ];
            Write(sub_yaml, i);
        }
    }

    void YamlEngine::Write(YAML::Node &yaml, const reflect::TypeView::Map &dict_parameter)
    {
        for (auto &it : dict_parameter)
        {
            YAML::Node sub_yaml = yaml[ it.first ];
            Write(sub_yaml, it.second);
        }
    }

    std::string YamlEngine::GetFilePath(const std::string &id) const
    {
        return root_ + "/" + id + ".yaml";
    }
}   // namespace poker::param::backend