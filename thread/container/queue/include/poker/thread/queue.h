//
// Created by zyk on 24-4-25.
//

#pragma once

#include <memory>
#include <queue>
#include <shared_mutex>


namespace poker::thread
{
    template < class T >
    class Queue
    {
    public:
        void push(const T &value);

        template < class... Args >
        void emplace(Args &&...args);

        bool try_pop(T &value);

        std::shared_ptr< T > try_pop();

        void wait_and_pop(T &value);

        std::shared_ptr< T > wait_and_pop();

    private:
        std::shared_mutex m_;

        std::queue< T > queue_;
    };
}   // namespace poker::thread