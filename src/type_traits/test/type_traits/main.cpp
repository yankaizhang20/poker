//
// Created by zyk on 24-6-4.
//

#include <utility>

#include <poker/type_traits.h>


struct A
{
    int x;
};

struct B
{
    bool x;
};

// clang-format off
poker_concept(X_Field_Is_Int, class T,
              (
                      std::is_same_v< decltype(std::declval< T >().x), int >
              )
             );
// clang-format on

template < class T, poker_requires(X_Field_Is_Int< T >::enable) >
void f()
{
}

template < class T >
class C
{
public:
    poker_enable_if(X_Field_Is_Int< T >::enable) static void f()
    {
    }
};

int main()
{
    f< A >();

    //    f< B >();

    C< A >::f();

    //    C< B >::f();

    return 0;
}