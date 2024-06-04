//
// Created by zyk on 24-5-9.
//

#pragma once
#include <cstddef>


namespace poker::system
{
    class LaunchCenter;

    class IComponent
    {
    public:
        virtual ~IComponent() = default;

    private:
        friend class LaunchCenter;

    private:
        /**
         * @brief 初始化
         */
        virtual void ModuleStart() = 0;

        /**
         * @brief 结束
         */
        virtual void ModuleShutdown() = 0;

        /**
         * @brief 指定启动优先级
         */
        virtual std::size_t GetLaunchPriority() = 0;
    };
}   // namespace poker::system