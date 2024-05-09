//
// Created by zyk on 24-5-9.
//

#pragma once

#include <unordered_map>

#include <poker/pattern/singleton.h>

#include "./interface/IComponent.h"


namespace poker::system
{
    class LaunchCenter
    {
    public:
        /**
         * @brief 注册组件到启动中心
         */
        void add(IComponent *component);

        /**
         * @brief 启动所有组件
         */
        void launch();

        /**
         * @brief 关闭所有组件
         */
        void shutdown();

    private:
        // 按优先级排序的各个组件
        std::unordered_multimap< std::size_t, IComponent * > comps_;
    };

    LaunchCenter &GetLaunchCenter();
}   // namespace poker::system