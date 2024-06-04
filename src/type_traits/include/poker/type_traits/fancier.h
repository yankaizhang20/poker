//
// Created by yarten on 2021/10/9.
//

#pragma once

#include <type_traits>

#include <poker/macro.h>


/**
 * @brief 模拟实现 c++ 20 requires 语法，静态推断类型，可用于模板参数列表或函数参数列表中。
 * 其中，括号内参数为一个编译时期可进行判断的表达式或 bool 值。
 *
 * @example
 * template<class T, poker_requires(std::is_same_v<T, int> or std::is_same_v<T, float>)>
 * T f()
 * {
 *     return T();
 * }
 *
 * f<int>();   // good
 * f<bool>();  // bad
 */
#define poker_requires(...) typename std::enable_if< (__VA_ARGS__), int >::type = 0


/**
 * @brief 模拟实现 c++ 20 concept 语法，创建一个可用复用的类型推断过程。
 * 其中，括号参数分为三个部分：
 * 1. concept 名字；
 * 2. concept 用到的模板类型声明（至少包括一个类型声明）；
 * 3. 静态的类型推断条件，需要使用括号包含起来，可使用 declvalue 配合使用；
 *
 * @example 单个类型的简单判断
 * poker_concept(MyConcept, class T,
 *     (
 *         std::is_same_v<T, int>  or
 *         std::is_same_v<T, float>
 *     )
 * );
 *
 * template<class T, poker_requires(MyConcept<T>::enable)>
 * void f() {}
 *
 * f<int>();  // good
 * f<bool>(); // bad
 *
 * @example 多个类型的判断
 * poker_concept(MyConcept, class T, class E, int N,
 *     (
 *         not std::is_same_v<T, E> and N != 0
 *     )
 * );
 *
 * template<class T, class E, int N, poker_requires(MyConcept<T, E, N>::enable)>
 * void f() {}
 *
 * f<int, float, 1>(); // good
 * f<int, float, 0>(); // bad
 * f<int, int  , 1>(); // bad
 *
 * @example 类成员的推断（使用 declvalue ）
 * // 定义 concept：
 * poker_concept(MyConcept， class T,
 *     (
 *         std::is_same_as< decltype( std::declval(T).x ), int >
 *     )
 * );
 *
 * template<class T, poker_requires(MyConcept<T>::enable)>
 * void f() {}
 *
 * struct A { int x; };
 * struct B { bool x; };
 *
 * f<A>();   // good
 * f<int>(); // bad
 * f<B>();   // bad
 *
 * @note 使用本宏定义的 concept 是可以级联使用的（放在 bool 表达式中），但需要注意的是，若使用到了
 * 类成员的推断，若类型中不存在该成员，则无论如何都会使推断失败（实际上是语法错误）。
 */
#define poker_concept(ConceptName, ...)                                                                            \
    template < POKER_ARGS_EXCEPT_LAST(1, __VA_ARGS__) >                                                            \
    struct ConceptName                                                                                             \
    {                                                                                                              \
        constexpr static bool enable =                                                                             \
                std::is_same_v< typename std::conditional< POKER_ARGS_THE_LAST(1, __VA_ARGS__), int, void >::type, \
                                int >;                                                                             \
    }


/**
 * @brief 用于修饰函数或类，静态推断它是否应该被使用。
 * @example
 * template<class T>
 * class A
 * {
 * public:
 *     nox_enable_if(std::is_same_v<T, int>)
 *     void f() {}
 * };
 *
 * A<int> a1;
 * a1.f();   // good
 *
 * A<float> a2;
 * a2.f();   // bad
 */
#define poker_enable_if(...) \
    template < typename Dummy = std::true_type, poker_requires(Dummy::value and (__VA_ARGS__)) >
