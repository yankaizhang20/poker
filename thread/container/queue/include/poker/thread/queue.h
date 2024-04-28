//
// Created by zyk on 24-4-25.
//

#pragma once

#include <condition_variable>
#include <memory>
#include <queue>
#include <shared_mutex>


namespace poker::thread
{
    template < class T >
    class Queue
    {
    public:
        void push(const T &value)
        {
            {
                std::lock_guard guard(m_);

                queue_.push(value);
            }
            con_.notify_one();
        }

        template < class... Args >
        void emplace(Args &&...args)
        {
            {
                std::lock_guard guard(m_);

                queue_.emplace(std::forward< Args >(args)...);
            }
            con_.notify_one();
        }

        bool try_pop(T &value)
        {
            std::lock_guard guard(m_);

            if (queue_.empty())
            {
                return false;
            }

            value = std::move(queue_.front());

            queue_.pop();

            return true;
        }

        std::shared_ptr< T > try_pop()
        {
            std::lock_guard guard(m_);

            if (queue_.empty())
            {
                return nullptr;
            }

            auto p_top = std::make_shared< T >(std::move(queue_.front()));

            queue_.pop();

            return p_top;
        }

        void wait_and_pop(T &value)
        {
            std::unique_lock guard(m_);

            con_.wait(guard, [ this ]() { return !queue_.empty(); });

            value = std::move(queue_.front());

            queue_.pop();
        }

        std::shared_ptr< T > wait_and_pop()
        {
            std::unique_lock guard(m_);

            con_.wait(guard, [ this ]() { return !queue_.empty(); });

            auto p_top = std::make_shared< T >(std::move(queue_.front()));

            queue_.pop();

            return p_top;
        }

    private:
        std::shared_mutex       m_;
        std::condition_variable con_;

        std::queue< T > queue_;
    };
}   // namespace poker::thread