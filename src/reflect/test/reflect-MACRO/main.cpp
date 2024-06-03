//
// Created by zyk on 24-5-30.
//

#include <poker/reflect.h>

namespace poker::reflect::test
{
    struct SubObject
    {
        int a = 0;
        int b = 3;
    };

    struct CompObject
    {
        SubObject sub_a;

        std::vector< SubObject > sub_b;
    };
}   // namespace poker::reflect::test

POKER_REFLECT(test::SubObject, a, b)
POKER_REFLECT(test::CompObject, sub_a, sub_b)

using namespace poker::reflect;

int main()
{
    test::CompObject obj;
    obj.sub_b.emplace_back();
    obj.sub_b.emplace_back();

    TypeView view;

    view.Bind(obj);

    return 0;
}