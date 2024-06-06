//
// Created by zyk on 2024-6-6
//

#pragma once

#include <functional>
#include <future>
#include <map>
#include <vector>

#include <poker/thread/Pool.h>
#include <poker/thread/lock.h>

#include "../CallbackResult.h"
#include "./name_hash.h"


namespace poker::databus::details
{
    // 解决 boost 函数，没有正确 move functor 的问题
    template < class T >
    struct PromiseWrapper
    {
        PromiseWrapper() = default;
        PromiseWrapper(const PromiseWrapper &other) : promise(std::move(const_cast< PromiseWrapper & >(other).promise))
        {
        }
        PromiseWrapper(PromiseWrapper &&) = default;

        std::promise< T > promise;
    };

    template < class TSignature >
    class CallbackImpl;

    template < class TRes, class... TArgs >
    class CallbackImpl< TRes(TArgs...) >
    {
    public:
        /**
         * @brief 用户提供的回调函处理数
         */
        using Handler = std::function< TRes(TArgs...) >;

        /**
         * @brief 全部回调处理函数得到结果后的处理函数
         */
        using ResultHandler = std::function< void(const CallbackResult< TRes > &) >;

    public:
        /**
         * @brief 设置最大的异步 Call 数量，配置的是线程池大小，当并发 Call 数量大于该值时，多余的 Call 任务将被延后。
         * @param size
         */
        void SetAsynchronousLoad(size_t size)
        {
            _async_call_pool.Resize(size);
        }

        void SetConcurrentLoad(size_t concurrent_load)
        {
            _concurrent_load = concurrent_load;

            if (_concurrently)
                _concurrent_call_semaphore.SetMaxCount(_concurrent_load);
        }

        /**
         * @brief 并发使能，允许不同线程的 Call() 函数并发执行
         */
        void EnableConcurrentCalls(bool enable)
        {
            _concurrently = enable;

            if (_concurrently)
                _concurrent_call_semaphore.SetMaxCount(_concurrent_load);
            else
                _concurrent_call_semaphore.SetMaxCount(1);
        }

        /**
         * @brief 并行使能，允许多个回调函数同时执行
         */
        void EnableParallelCallbacks(bool enable)
        {
            _parallel = enable;
        }

        /**
         * @brief 允许 Call() 函数阻塞或非并发时，丢弃需要等待的 Call
         */
        void EnableDiscardCalls(bool enable)
        {
            _can_discard = enable;
        }

    public:
        /**
         * @brief 追加处理函数，将自动使用当前的顺序作为该处理函数的编号
         */
        void AddCallback(Handler handler)
        {
            AddCallback(_handlers.size(), std::move(handler));
        }

        /**
         * @brief 追加指定编号的处理函数
         * @param n 指定的编号，可能覆盖已有相同编号的处理函数，并更新回调序列。
         */
        void AddCallback(std::size_t n, Handler handler)
        {
            Locking(_handlers) _handlers.push_back(n, std::move(handler));
        }

        /**
         * @brief 追加指定名字的处理函数
         * @param handler
         * @param name 将通过哈希函数，将字符串名称处理为编号。
         */
        void AddCallback(const std::string &name, Handler handler)
        {
            AddCallback(name_hash(name), std::move(handler));
        }

        /**
         * @brief 删除指定编号的处理函数
         * @param n
         */
        void RemoveCallback(std::size_t n)
        {
            Locking(_handlers) _handlers.erase_by_key(n);
        }

        /**
         * @brief 删除指定名称的处理函数
         * @param name
         */
        void RemoveCallback(const std::string &name)
        {
            RemoveCallback(name_hash(name));
        }

        /**
         * @brief 删除全部的处理函数
         */
        void RemoveAllCallbacks()
        {
            Locking(_handlers) _handlers.clear();
        }

        /**
         * @brief 设置回调处理函数返回结果的处理函数
         * @param handler
         */
        void SetResultHandler(ResultHandler handler)
        {
            Locking(_result_handler) _result_handler = std::move(handler);
        }

    public:
        /**
         * @see Call() 函数
         * @param args
         */
        CallbackResult< TRes > operator() (TArgs... args)
        {
            return Call(std::forward< TArgs >(args)...);
        }

        /**
         * @brief 主动式的回调处理调用，由控制反转的模块调用，处理 Call 的异步调用
         * @param args
         */
        std::future< CallbackResult< TRes > > AsyncCall(TArgs... args)
        {
            // 异步启动调用线程；若当前异步处理达到上限（也即当前已经有回调函数在运行），同时
            // 用户设置了 drop 使能，则放弃该回调处理。
            if (_async_call_pool.RemainingTasksCount() >= _async_call_pool.Size() and _can_discard)
                return {};

            PromiseWrapper< CallbackResult< TRes > > promise_wrapper;
            std::future< CallbackResult< TRes > >    future = promise_wrapper.promise.get_future();

            _async_call_pool.Add(
                    [ this, &promise_wrapper, args = std::make_tuple(std::forward< TArgs >(args)...) ]()
                    {
                        return std::apply([ this, &promise_wrapper ](auto &&...args)
                                          { MainAsyncCall(promise_wrapper, std::forward< decltype(args) >(args)...); },
                                          std::move(args));
                    });

            return future;
        }

        /**
         * @brief 主动式的回调处理调用，由控制反转模块调用，处理 Call 的同步调用
         * @param args
         * @return 处理结果，可能为空。
         */
        CallbackResult< TRes > Call(TArgs... args)
        {
            return WaitCall(poker::unit::Time::zero, std::forward< TArgs >(args)...);
        }

        /**
         * @brief 同步式调用，允许一定的阻塞等待时间
         * @param waiting_time 阻塞等待进入 Call 的时长
         * @param args
         * @return 处理结果，可能为空。
         */
        CallbackResult< TRes > WaitCall(const poker::unit::Time &waiting_time, TArgs... args)
        {
            CallbackResult< TRes > result;

            if (_concurrent_call_semaphore.TryAcquire())
            {
                // 以非阻塞的方式申请进行 Call 流程，若成功申请，则执行后续流程
                goto EXECUTE_MAIN_CALL;
            }

            // 无法立即获得 Call 的权限，则判断是否设置了 discard 参数，
            // 若设置该参数，意味着当无法立即 Call 时，直接返回空结果。
            if (_can_discard)
                goto RETURN_RESULT;

            // 若不可以抛弃这次 Call 流程，则进行阻塞等待权限；
            // 若设置了阻塞等待时间，则在一定时间内进行等待：
            if (waiting_time.Get< pokeru::us >() > 0)
            {
                // 在等待的时间内获得了 Call 权限，则进行 Call，否则返回空结果
                if (_concurrent_call_semaphore.Acquire(waiting_time))
                    goto EXECUTE_MAIN_CALL;
                else
                    goto RETURN_RESULT;
            }
            else
            {
                // 没有设置等待时间，则持续等待，直至该对象被析构。
                if (_concurrent_call_semaphore.Acquire())
                    goto EXECUTE_MAIN_CALL;
                else
                    goto RETURN_RESULT;
            }

        EXECUTE_MAIN_CALL:
            result = MainCall(std::forward< TArgs >(args)...);

            _concurrent_call_semaphore.Release();

        RETURN_RESULT:
            return result;
        }

        /**
         * @brief 阻塞等待所有线程处理结束
         */
        void Wait()
        {
            _async_call_pool.Wait();
        }

        /**
         * @return 回调处理函数的数量
         */
        [[nodiscard]]
        size_t Size() const
        {
            return _handlers.size();
        }

        /**
         * @return 回调处理函数的数量是否为零
         */
        [[nodiscard]]
        bool Empty() const
        {
            return _handlers.empty();
        }

        /**
         * @brief 本类的析构过程不完备，需要在本类的子类（Callback）中回收资源。
         */
        virtual ~CallbackImpl() = default;

    protected:
        /**
         * @brief 进行回调，并将结果写入到 result 变量中，返回回调是否被拦截。由于该功能取决于返回值类型，
         * 当返回值类型为 void 时，将不需要写入结果，且回调无法被拦截。为了区分返回值类型，该函数的主要实现
         * 交由不同特化的子类来处理。
         * @return true 为回调被拦截。
         */
        virtual bool CallOneHandler(const Handler &handler, CallbackResult< TRes > &result, std::size_t result_index,
                                    TArgs... args) = 0;

        /**
         * @brief 可由子类特化实现，当返回值非空时，要求根据 callback 的索引值，逐一初始化结果容器
         * @return 默认的空结果
         */
        virtual CallbackResult< TRes > CreateEmptyCallbackResult(const std::vector< std::size_t > &callback_keys) = 0;

        /**
         * @brief 用于子类析构时调用，回收所有资源。
         */
        void Shutdown()
        {
            RemoveAllCallbacks();
            _async_call_pool.Wait();
        }

    private:
        /**
         * @brief 异步回调处理的主要过程，屏蔽了返回值
         * @param args
         */
        void MainAsyncCall(PromiseWrapper< CallbackResult< TRes > > promise_wrapper, TArgs... args)
        {
            auto result = Call(std::forward< TArgs >(args)...);
            promise_wrapper.promise.set_value(result);
        }

        CallbackResult< TRes > MainCall(TArgs... args)
        {
            // 拷贝回调处理函数，防止时间锁定回调函数，使用户设置回调函数的接口被长时间阻塞
            HandlerArray handlers;

            Locking(_handlers) handlers = (HandlerArray &) _handlers;

            // 创建空结果
            CallbackResult< TRes > result = CreateEmptyCallbackResult(handlers.keys());

            if (_parallel)
            {
                // 计算当前理论上使全部回调函数并发执行的线程池数量
                std::size_t min_parallel_count_for_all_callbacks = _concurrent_load * handlers.size();

                // 自适应扩大并发回调线程池的大小
                if (min_parallel_count_for_all_callbacks > _parallel_callback_pool.Size())
                    _parallel_callback_pool.Resize(min_parallel_count_for_all_callbacks);

                // 创建该消息的所有回调处理的任务组
                thread::TaskGroup task_group;

                for (size_t i = 0, size = handlers.size(); i < size; ++i)
                {
                    task_group += _parallel_callback_pool.Add([ &, i ]
                                                              { CallOneHandler(handlers[ i ], result, i, args...); });
                }

                // 等待该组消息全部处理完毕
                task_group.Wait();
            }
            else
            {
                // 非并发执行回调处理函数，则按顺序执行
                for (size_t i = 0, size = handlers.size(); i < size; ++i)
                {
                    if (bool intercepted = CallOneHandler(handlers[ i ], result, i, args...); intercepted)
                        break;
                }
            }

            // 拷贝回调结果的处理函数，并调用它
            ResultHandler result_handler;

            Locking(_result_handler) result_handler = (ResultHandler &) _result_handler;

            if (result_handler)
                result_handler(result);

            // 返回最终的结果
            return result;
        }

    private:
        // 默认仅有一个线程的线程池，用于异步回调
        thread::Pool _async_call_pool = thread::Pool(1);

        // 用于并发调用用户回调处理函数的线程池
        thread::Pool _parallel_callback_pool;

        // 控制回调函数的策略
        std::atomic_bool _concurrently = false;   // 是否允许多个异步 Call 过程并发执行
        std::atomic_bool _parallel     = false;   // 是否允许用户回调函数并行执行
        std::atomic_bool _can_discard =
                true;   // 若 Call 阻塞或非并发执行时（也即当前需要等待别的 Call 流程结束），是否放弃 Call
        std::atomic_size_t _concurrent_load = 1;   // 并发 Call 的最大数量

        // 用于控制并发数量的互斥量
        poker::thread::Semaphore _concurrent_call_semaphore;

        // 用于同步回调处理
        thread::MutexLock _calling_lock;

        // 正在回调处理的线程数量
        thread::MutexLock _calling_count_lock;
        int               _calling_count = 0;

        // 用于并发调用用户

        // 用户设置的回调函数，使用有序的编号进行管理
        using HandlerArray = std::map< std::size_t, Handler >;
        MutexLockable(HandlerArray) _handlers;

        // 用户回调函数返回结果的回调处理函数
        MutexLockable(ResultHandler) _result_handler;
    };
}   // namespace poker::databus::details
