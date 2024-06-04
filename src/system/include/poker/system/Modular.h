//
// Created by zyk on 24-6-4.
//

#pragma once

#include <poker/pattern.h>
#include <poker/type_traits.h>

#include "./AutoComponent.h"


namespace poker::system
{
    template < class FinalType, class... TComponents >
    class Modular : public AutoComponent< FinalType >
    {
        static_assert(traits::is_different_from_v< FinalType, TComponents... >);
        static_assert(traits::is_all_unique_v< TComponents... >);

    protected:
        template < class T >
        T &core()
        {
            static_assert(not traits::is_different_from_v< T, TComponents... >);
            return pattern::GetGlobalUnique< T >::Ref();
        }
    };

    template < class FinalType, class T >
    class Modular< FinalType, T > : public AutoComponent< FinalType >
    {
    protected:
        T &core()
        {
            return pattern::GetGlobalUnique< T >::Ref();
        }

        static T &unique_core;
    };

    template < class FinalType, class T >
    T &Modular< FinalType, T >::unique_core = pattern::GetGlobalUnique< T >::Ref();

    template < class FinalType >
    class Modular< FinalType > : public AutoComponent< FinalType >
    {
    };
}   // namespace poker::system