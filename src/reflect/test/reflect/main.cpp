//
// Created by zyk on 24-5-30.
//

#include <poker/reflect/entity/ReflectType.h>
#include <poker/reflect/entity/TypeView.h>
#include <poker/reflect/entity/ValueType.h>


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

namespace poker::reflect
{
    template <>
    void Bind< test::SubObject >(test::SubObject &obj, TypeView &view)
    {
        view.AddField("a", obj.a);
        view.AddField("b", obj.b);
    }

    template <>
    void Bind< test::CompObject >(test::CompObject &obj, TypeView &view)
    {
        view.AddField("sub_a", obj.sub_a);
        view.AddField("sub_b", obj.sub_b);
    }
}   // namespace poker::reflect

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