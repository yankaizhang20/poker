//
// Created by zyk on 24-4-25.
//

#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>


namespace poker::thread
{
    template < class T >
    class Queue
    {
    public:
        Queue() : head_(new Node()), tail_(head_)
        {
        }
        Queue(const Queue &other)
        {
        }

    public:
        void push(const T &value)
        {
            auto p_dummy_node = new Node();

            std::lock_guard guard(t_m_);

            // 在原有尾部构造新数据
            tail_->p_data = std::make_shared< T >(value);
            tail_->next   = p_dummy_node;

            // 使 tail 指向新的傀儡节点
            tail_ = p_dummy_node;
        }

        template < class... Args >
        void emplace(Args &&...args)
        {
        }

        bool try_pop(T &value)
        {
            std::lock_guard guard(h_m_);

            // 若为初始状态，则返回 false

            auto old_head = head_;
            value         = *old_head->p_data;

            head_ = old_head->next;

            delete old_head;
        }

        std::shared_ptr< T > try_pop()
        {
        }

        void wait_and_pop(T &value)
        {
        }

        std::shared_ptr< T > wait_and_pop()
        {
        }

    private:
        struct Node
        {
            std::shared_ptr< T > p_data = nullptr;
            Node                *next   = nullptr;
        };

    private:
        std::mutex              h_m_, t_m_;
        std::condition_variable con_;

        bool is_init = true;

        Node *head_, *tail_;
    };
}   // namespace poker::thread