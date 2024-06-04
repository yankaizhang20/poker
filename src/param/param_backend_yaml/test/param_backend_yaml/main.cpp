//
// Created by zyk on 24-6-3.
//

#include <poker/param/backend/yaml.h>


struct SubObject
{
    int         a = 0;
    int         b = 0;
    std::string str;
};

POKER_REFLECT_TYPE(SubObject, a, b, str)

enum class TestEnum
{
    undefined,
    one,
    two,
    three
};

POKER_REFLECT_ENUM(TestEnum, undefined, one, two, three)

struct CompObject
{
    TestEnum sub_enum = TestEnum::undefined;

    SubObject sub_struct;

    std::vector< SubObject > sub_vector;

    std::list< SubObject > sub_list;

    std::map< std::string, SubObject > sub_map;
};

POKER_REFLECT_TYPE(CompObject, sub_enum, sub_struct, sub_vector, sub_list, sub_map)


void SetCompObject(CompObject &obj)
{
    auto SetSubObject = [](SubObject &obj)
    {
        obj.a   = 1;
        obj.b   = 1;
        obj.str = "1";
    };

    obj.sub_enum = TestEnum::three;

    SetSubObject(obj.sub_struct);

    for (auto &sub : obj.sub_vector)
    {
        SetSubObject(sub);
    }

    for (auto &sub : obj.sub_list)
    {
        SetSubObject(sub);
    }

    for (auto &sub : obj.sub_map)
    {
        SetSubObject(sub.second);
    }
}

int main()
{
    // 使用 YamlEngine
    poker::param::backend::YamlEngine engine;
    engine.SetFileRoot(".");

    // 设置参数后端
    poker::param::backend::SetReader(engine.GetReader());
    poker::param::backend::SetWriter(engine.GetWriter());

    // 创建原始类型
    CompObject obj;
    obj.sub_vector.emplace_back();
    obj.sub_vector.emplace_back();
    obj.sub_list.emplace_back();
    obj.sub_list.emplace_back();
    obj.sub_map.emplace("one", SubObject {});
    obj.sub_map.emplace("two", SubObject {});

    SetCompObject(obj);

    TestEnum en = TestEnum::three;

    // 写参数
    poker::param::Write("test_param_interface_type", obj);
    poker::param::Write("test_param_interface_enum", en);

    // 读参数
    CompObject obj_from_read;
    TestEnum   en_from_read;

    poker::param::Read("test_param_interface_type", obj_from_read);
    poker::param::Read("test_param_interface_enum", en_from_read);

    return 0;
}
