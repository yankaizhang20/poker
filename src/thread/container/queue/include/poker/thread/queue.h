//
// Created by zyk on 24-4-25.
//

#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>

#include <poker/thread/lock.h>


/**
 * @brief 更细粒度的线程安全队列，可实现压入、弹出的同步进行（大部分）
 */
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

            copy_list(other.p_head_.get(), other.p_tail_);
        }

    public:
        void push(const T &value)
        {
            // 提前构造新数据
            auto p_data = std::make_shared< T >(value);

            // 提前构造傀儡节点
            std::unique_ptr< Node > p_dummy_node(new Node());

            // 修改队列
            Locking(t_m_)
            {
                // 在原有尾部构造新数据
                p_tail_->p_data = std::move(p_data);
                p_tail_->next   = p_dummy_node;

                // 使 tail 指向新的傀儡节点
                p_tail_ = p_dummy_node.get();
            }

            // 通知队列已经非空
            has_new_node_.notify_one();
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
            std::shared_ptr< T >    p_data = nullptr;
            std::unique_ptr< Node > next   = nullptr;
        };

        const Node *get_tail()
        {
            Locking(t_m_)
            {
                return p_tail_;
            }
        }

        std::unique_ptr< Node > pop_head()
        {
            auto old_head = std::move(p_head_);

            p_head_ = std::move(old_head->next);

            return old_head;
        }

        std::unique_ptr< Node > try_pop_head()
        {
            Locking(h_m_)
            {
                // 若为初始状态，则返回 false
                if (p_head_.get() == get_tail())
                {
                    return nullptr;
                }

                return pop_head();
            }
        }

        std::unique_ptr< Node > wait_pop_head()
        {
            std::unique_lock guard(h_m_);

            has_new_node_.wait(guard, [ this ]() { return p_head_.get() != get_tail(); });

            return pop_head();
        }

    private:
        void copy_list(const Node *p_head, const Node *p_tail)
        {
            const Node *another_cur = p_head;

            while (another_cur != p_tail)
            {
                this->push(*another_cur->p_data);

                another_cur = another_cur->next.get();
            }
        }

    private:
        std::condition_variable has_new_node_;

        // 队列头部
        MutexLock                h_m_;
        std::unique_lock< Node > p_head_;

        // 队列尾部，永远指向一个傀儡节点
        MutexLock t_m_;
        Node     *p_tail_;
    };
}   // namespace poker::thread