//
// Created by zyk on 24-2-21.
//

#include <type_traits>


namespace fake_std
{
    template < bool, class Type >
    struct enable_if
    {
    };

    template < class Type >
    struct enable_if< true, Type >
    {
        using type = Type;
    };
}   // namespace fake_std


namespace use_case
{
    struct BBB
    {
    };

    template < class Type, typename fake_std::enable_if< std::is_base_of_v< BBB, Type >, int >::type = 0 >
    void func(Type a)
    {
    }

    struct TTT : public BBB
    {
        static bool value;
    };

    bool TTT::value = true;

    struct SSS
    {
    };

    void test_case1()
    {
        use_case::TTT a;
        func(a);
    };

    void test_case2()
    {
        use_case::SSS b;
        func(b);
    }
}   // namespace use_case


int main()
{
    use_case::test_case1();

    return 0;
}