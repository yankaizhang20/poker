//
// Created by zyk on 24-5-9.
//

#pragma once

#include <iostream>

#include <poker/macro.h>
#include <poker/system/launch_center.h>


namespace poker
{
    /**
     * @brief 自启动组件，继承该类可实现模块自启动
     * @tparam T 需要自启动的类型
     */
    template < class T >
    class AutoComponent : public system::IComponent
    {
    public:
        AutoComponent()
        {
            // 防止编译器优化掉 helper_;
            (void) helper_;
        }

    private:
        static char helper_;
    };

    template < class T >
    char AutoComponent< T >::helper_ = []()
    {
        system::GetLaunchCenter().add(pattern::GetGlobalUnique< T >::Ptr());
        return 0;
    }();

/**
 * @brief 防止自启动类型 T 由于没有在 main 中使用而被编译器优化
 * @note 需在类型定义之外使用该宏
 */
#define POKER_NO_OPTIMIZED(T)                                                \
    namespace poker::helper                                                  \
    {                                                                        \
        static const char POKER_COUNTER_UNIQUE(auto_component_helper) = []() \
        {                                                                    \
            pattern::GetGlobalUnique< T >::Ref();                            \
            return 0;                                                        \
        }();                                                                 \
    }

}   // namespace poker