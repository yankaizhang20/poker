//
// Created by zyk on 24-6-3.
//

#include <poker/param/backend/yaml.h>


struct SubObject
{
    int a = 0;
    int b = 0;
};

struct CompObject
{
    SubObject sub_a;

    std::vector< SubObject > sub_b;
};

POKER_REFLECT_TYPE(SubObject, a, b)
POKER_REFLECT_TYPE(CompObject, sub_a, sub_b)


enum class TestEnum
{
    one,
    two,
    three
};

POKER_REFLECT_ENUM(TestEnum, one, two, three)


void SetCompObject(CompObject &obj)
{
    auto SetSubObject = [](SubObject &obj)
    {
        obj.a = 1;
        obj.b = 1;
    };

    SetSubObject(obj.sub_a);

    for (auto &sub : obj.sub_b)
    {
        SetSubObject(sub);
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

    CompObject obj;
    obj.sub_b.emplace_back();
    obj.sub_b.emplace_back();

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
