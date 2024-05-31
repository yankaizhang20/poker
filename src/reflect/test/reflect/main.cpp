//
// Created by zyk on 24-5-30.
//

#include <poker/reflect/entity/TypeView.h>
#include <poker/reflect/entity/meta.h>


namespace poker::reflect::test
{
    struct Object
    {
        int a = 0;
        int b = 3;
    };
}   // namespace poker::reflect::test

namespace poker::reflect
{
    template <>
    void Bind< test::Object >(test::Object &obj, TypeView &view)
    {
        view.AddField("a", obj.a);
        view.AddField("b", obj.b);
    }
}   // namespace poker::reflect

using namespace poker::reflect;

int main()
{
    test::Object obj;
    TypeView     view;

    view.Bind(obj);

    return 0;
}