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
        Queue() : p_head_(new Node()), p_tail_(p_head_.get())
        {
        }
        Queue(const Queue &other)
        {
            std::scoped_lock guard(other.h_m_, other.t_m_);

            copy_list(other.p_head_.get());
        }

    public:
        void push(const T &value)
        {
            // 提前构造新数据
            auto p_data = std::make_shared< T >(value);

            // 提前构造傀儡节点
            std::unique_lock< Node > p_dummy_node(new Node());

            {
                std::lock_guard guard(t_m_);

                // 在原有尾部构造新数据
                p_tail_->p_data = std::move(p_data);
                p_tail_->next   = std::move(p_dummy_node);

                // 使 tail 指向新的傀儡节点
                p_tail_ = p_dummy_node.get();
            }

            con_.notify_one();
        }

        template < class... Args >
        void emplace(Args &&...args)
        {
        }

        bool try_pop(T &value)
        {
            auto p_head = try_pop_head();

            if (!p_head)
            {
                return false;
            }

            value = *p_head->p_data;
            return true;
        }

        std::shared_ptr< T > try_pop()
        {
            auto p_head = try_pop_head();

            return p_head ? p_head->p_data : nullptr;
        }

        void wait_and_pop(T &value)
        {
            auto p_head = wait_and_pop();

            value = *p_head->p_data;
        }

        std::shared_ptr< T > wait_and_pop()
        {
            auto p_head = wait_and_pop();

            return p_head->p_data;
        }

    private:
        struct Node
        {
            std::shared_ptr< T >     p_data = nullptr;
            std::unique_lock< Node > next   = nullptr;
        };

        Node *get_tail()
        {
            std::lock_guard guard(t_m_);

            return p_tail_;
        }

        std::unique_lock< Node > pop_head()
        {
            auto old_head = std::move(p_head_);

            p_head_ = std::move(old_head->next);

            return old_head;
        }

        std::unique_lock< Node > try_pop_head()
        {
            std::lock_guard guard(h_m_);

            // 若为初始状态，则返回 false
            if (p_head_.get() == get_tail())
            {
                return nullptr;
            }

            return pop_head();
        }

        std::unique_lock< Node > wait_pop_head()
        {
            std::unique_lock guard(h_m_);

            con_.wait(guard, [ this ]() { return p_head_.get() != get_tail(); });

            return pop_head();
        }

    private:
        void copy_list(const std::unique_lock< Node > &p_head)
        {
            std::unique_lock< Node > cur = p_head_, another_cur = p_head;

            while (cur and another_cur)
            {
                cur =
            }
        }

    private:
        std::mutex              h_m_, t_m_;
        std::condition_variable con_;

        std::unique_lock< Node > p_head_;
        Node                    *p_tail_;
    };
}   // namespace poker::thread