//
// Created by zyk on 2024-6-6
//

#pragma once

#include <poker/macro.h>

#include "./callback/CallbackImpl.h"


namespace poker::databus::details
{
    template < class TSignature >
    class Callback;

    template < class TRes, class... TArgs >
    class Callback< TRes(TArgs...) > : public CallbackImpl< TRes(TArgs...) >
    {
        using Base    = CallbackImpl< TRes(TArgs...) >;
        using Handler = typename CallbackImpl< TRes(TArgs...) >::Handler;

    public:
        /**
         * @brief 启用拦截策略。若用户回调处理函数返回值与之给定的相同时，拦截后续的回调处理。（仅顺序执行时有效）
         * @param result
         */
        void EnableInterception(TRes result)
        {
            Locking(_interception) _interception.template emplace(result);
        }

        /**
         * @brief 禁用拦截策略。
         */
        void DisableInterception()
        {
            Locking(_interception) _interception.reset();
        }

        ~Callback() override
        {
            Base::Shutdown();
        }

    protected:
        bool CallOneHandler(const Handler &handler, CallbackResult< TRes > &result, std::size_t result_index,
                            TArgs... args) final
        {
            if (handler)
            {
                result[ result_index ] = handler(args...);

                if (_interception.has_value() and result[ result_index ].value() == _interception.value())
                {
                    result.intercepted = true;
                    return true;
                }
            }

            return false;
        }

        CallbackResult< TRes > CreateEmptyCallbackResult(const std::vector< std::size_t > &callback_keys) final
        {
            CallbackResult< TRes > result;

            for (auto id : callback_keys)
                result.push_back(id, {});

            result.intercepted = false;

            return result;
        }

    private:
        MutexLockable(std::optional< TRes >) _interception;
    };

    template < class... TArgs >
    class Callback< void(TArgs...) > : public CallbackImpl< void(TArgs...) >
    {
        using Base    = CallbackImpl< void(TArgs...) >;
        using Handler = typename CallbackImpl< void(TArgs...) >::Handler;

    public:
        ~Callback() override
        {
            Base::Shutdown();
        }

    protected:
        bool CallOneHandler(const Handler &handler, CallbackResult< void > &, std::size_t, TArgs... args) final
        {
            if (handler)
                handler(args...);

            return false;
        }

        CallbackResult< void > CreateEmptyCallbackResult(const std::vector< std::size_t > &) final
        {
            return {};
        }
    };
}   // namespace poker::databus::details
