//
// Created by zyk on 24-5-30.
//

#include <poker/reflect/entity/TypeView.h>
#include <poker/reflect/entity/meta.h>


namespace poker::reflect::test
{
    struct Object
    {
        int    a = 0;
        double b = 3.0;
    };
}   // namespace poker::reflect::test

using namespace poker::reflect;

int main()
{
    TypeView view;

    test::Object obj;

    view.AddField("a", obj.a);
    view.AddField("b", obj.b);
}