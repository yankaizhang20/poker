//
// Created by zyk on 24-6-5.
//

#pragma once

#include <type_traits>


namespace poker::traits
{
    /**
     * @brief 判断第一个类型是否与剩余的所有类型都不相同；若都不相同，则其 value 为 true.
     */
    template < class CheckedType, class... Types >
    struct is_different_from;

    template < class CheckedType, class T1, class... Rest >
    struct is_different_from< CheckedType, T1, Rest... >
    {
        constexpr static bool value =
                is_different_from< CheckedType, T1 >::value and is_different_from< CheckedType, Rest... >::value;
    };

    template < class CheckedType, class T1 >
    struct is_different_from< CheckedType, T1 >
    {
        constexpr static bool value = not std::is_same_v< CheckedType, T1 >;
    };

    template < class CheckedType >
    struct is_different_from< CheckedType >
    {
        constexpr static bool value = true;
    };

    template < class CheckedType, class... Types >
    constexpr static bool is_different_from_v = is_different_from< CheckedType, Types... >::value;

    /**
     * @brief 判断给定类型中，是否有重复出现的类型；如果没有重复的类型，则其 value 为 true.
     */
    template < class... Types >
    struct is_all_unique;

    template < class T1, class T2, class... Rest >
    struct is_all_unique< T1, T2, Rest... >
    {
        constexpr static bool value = is_different_from_v< T1, T2, Rest... > and is_all_unique< T2, Rest... >::value;
    };

    template < class T1, class T2 >
    struct is_all_unique< T1, T2 >
    {
        constexpr static bool value = not std::is_same_v< T1, T2 >;
    };

    template < class T >
    struct is_all_unique< T >
    {
        constexpr static bool value = true;
    };

    template <>
    struct is_all_unique<>
    {
        constexpr static bool value = true;
    };

    template < class... Types >
    constexpr static bool is_all_unique_v = is_all_unique< Types... >::value;

    /**
     * @brief 统计类型的数量。
     */
    template < class... Types >
    struct number_of;

    template < class T, class... Rest >
    struct number_of< T, Rest... >
    {
        constexpr static size_t value = 1 + number_of< Rest... >::value;
    };

    template < class T >
    struct number_of< T >
    {
        constexpr static size_t value = 1;
    };

    template <>
    struct number_of<>
    {
        constexpr static size_t value = 0;
    };

    template < class... Types >
    constexpr static size_t number_of_v = number_of< Types... >::value;
}   // namespace poker::traits