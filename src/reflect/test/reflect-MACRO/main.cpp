//
// Created by zyk on 24-5-30.
//

#include <poker/reflect.h>

struct SubObject
{
    int a = 0;
    int b = 3;
};

struct CompObject
{
    SubObject sub_struct;

    std::vector< SubObject > sub_vector;
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


using namespace poker::reflect;

int main()
{
    // 绑定类型
    CompObject obj;
    obj.sub_vector.emplace_back();
    obj.sub_vector.emplace_back();

    TypeView obj_view;

    obj_view.Bind(obj);

    // 绑定枚举
    TestEnum en = TestEnum::three;

    TypeView en_view;

    en_view.Bind(en);

    return 0;
}