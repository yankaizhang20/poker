//
// Created by zyk on 24-4-24.
//

#pragma once

#include <optional>
#include <shared_mutex>
#include <stack>


namespace poker::thread
{
    template < class T >
    class Stack
    {
    public:
        void push(T data)
        {
            std::lock_guard guard(m_);

            data_.push_back(std::shared_ptr< T >(new T(std::move(data))));
        }

        template < class... Args >
        void emplace(Args &&...args)
        {
            std::lock_guard guard(m_);

            data_.emplace(std::shared_ptr< T >(new T(std::forward< Args >(args)...)));
        }

        bool try_pop(T &data)
        {
            std::lock_guard guard(m_);

            if (data_.empty())
            {
                return false;
            }

            data = std::move(*data_.top());

            data_.pop();
        }

        std::optional< std::shared_ptr< T > > try_pop()
        {
            std::lock_guard guard(m_);

            if (data_.empty())
            {
                return {};
            }

            auto top = data_.top();

            data_.pop();

            return top;
        }

    private:
        std::shared_mutex m_;

        // TODO:使用哪种数据结构类实现的依据，这里仅仅是因为 stack 提供了相应的接口而已
        std::stack< std::shared_ptr< T > > data_;
    };
}   // namespace poker::thread