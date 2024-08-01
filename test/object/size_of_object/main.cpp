//
// Created by zyk on 24-8-1.
//

#include <iostream>


namespace test_virtual_base
{
    class X
    {
    };

    class Y : public virtual X
    {
    };

    class Z : public virtual X
    {
    };

    class A : public Y, public Z
    {
    };

    void test()
    {
        std::cout << std::endl;

        std::cout << "sizeof(X)=" << sizeof(X) << std::endl
                  << "sizeof(Y)=" << sizeof(Y) << std::endl
                  << "sizeof(Z)=" << sizeof(Z) << std::endl
                  << "sizeof(A)=" << sizeof(A) << std::endl;
        /*
            sizeof(X)=1  // 确保每个 object 有唯一地址
            sizeof(Y)=8  // 虚基类表指针+虚基类优化+无需对齐
            sizeof(Z)=8
            sizeof(A)=16 // 两个子 object 的虚基表
         */
    }
}   // namespace test_virtual_base


namespace test_virtual_base_and_virtual_func
{
    class X
    {
    public:
        virtual void done() = 0;
    };

    class Y : public virtual X
    {
    public:
        virtual void done_of_Y() = 0;
    };

    class Z : public virtual X
    {
    };

    class A : public Y, public Z
    {
    };

    void test()
    {
        std::cout << std::endl;

        std::cout << "sizeof(X)=" << sizeof(X) << std::endl
                  << "sizeof(Y)=" << sizeof(Y) << std::endl
                  << "sizeof(Z)=" << sizeof(Z) << std::endl
                  << "sizeof(A)=" << sizeof(A) << std::endl;
        /*
            sizeof(X)=8 // 虚函数表指针
            sizeof(Y)=8 // 不存在虚基表指针
            sizeof(Z)=8
            sizeof(A)=16
         */
    }
}   // namespace test_virtual_base_and_virtual_func


namespace test_virtual_func_table_ptr_pos
{
    class X
    {
    public:
        virtual void done(){};

        friend void test();

    protected:
        int x=0, y=0, z=0;
    };

    void test()
    {
        std::cout << std::endl;

        printf("offset of x=%p\n", &X::x);
        printf("offset of y=%p\n", &X::y);
        printf("offset of z=%p\n", &X::z);

        std::cout << "offset of x=" << &X::x << std::endl
                  << "offset of y=" << &X::y << std::endl
                  << "offset of z=" << &X::z << std::endl;

        X obj;
        int X::*ptr=&X::x;

        std::cout << "通过成员指针存取成员" <<  obj.*ptr << std::endl;

        /*
            offset of x=0x8 // 虚表指针在放在了最开始
            offset of y=0xc
            offset of z=0x10
            offset of x=1
            offset of y=1
            offset of z=1
         */
    }
}   // namespace test_virtual_func_table_ptr_pos

int main()
{
    std::cout << "sizeof(int)=" << sizeof(int) << std::endl;
    std::cout << "sizeof(int*)=" << sizeof(int *) << std::endl;

    std::cout << std::endl;

    test_virtual_base::test();

    test_virtual_base_and_virtual_func::test();

    test_virtual_func_table_ptr_pos::test();

    return 0;
}