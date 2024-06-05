//
// Created by zyk on 24-6-5.
//

#pragma once

#include <functional>

#include <poker/thread/Condition.h>
#include <poker/unit.h>


namespace poker::thread
{
    /**
     * @brief 循环线程类
     */
    class Loop
    {
    public:
        /**
         * @brief 启动循环线程，将按循环线程的生命周期开始运行：开始 -> 主循环 -（收到结束信号）-> 结束。
         */
        void Start();

        /**
         * @brief 暂停主循环
         */
        void Pause();

        /**
         * @brief 恢复主循环
         */
        void Resume();

        /**
         * @brief 要求循环线程结束
         * @param wait 是否阻塞等待其结束
         */
        void Stop(bool wait = true);

        /**
         * @brief 等待循环线程结束
         */
        void Wait();

        /**
         * @brief 在指定时间内，等待循环线程结束
         * @param time
         * @return 本函数返回时，该循环线程是否已经结束
         */
        bool Wait(const poker::unit::Time &time);

        /**
         * @brief 等待进入主循环（也即结束了[开始流程]）
         */
        void WaitStarted();

        /**
         * @brief 在指定时间内，等待进入主循环
         * @param time
         * @return 本函数返回时，该循环线程已经进入主循环
         */
        bool WaitStarted(const poker::unit::Time &time);

        /**
         * @brief 析构函数，将自动结束本循环线程，并阻塞等待直到全部流程结束，但不会回调任何的生命周期回调函数。
         */
        virtual ~Loop();

    public:
        /**
         * @brief 设置主循环执行频率
         * @param hz
         */
        Loop &SetFrequency(double hz);

        /**
         * @brief 设置主循环执行周期
         * @param time
         */
        Loop &SetPeriod(const poker::unit::Time &time);

        /**
         * @brief 设置主循环以最快速度运行
         */
        Loop &SetAsFastAsPossible();

        template < class Fx, class... Args >
        Loop &SetOnStart(Fx &&fx, Args &&...args)
        {
            _on_start = std::bind(std::forward< Fx >(fx), std::forward< Args >(args)...);
            return *this;
        }

        template < class Fx, class... Args >
        Loop &SetOnRun(Fx &&fx, Args &&...args)
        {
            _on_run = std::bind(std::forward< Fx >(fx), std::forward< Args >(args)...);
            return *this;
        }

        template < class Fx, class... Args >
        Loop &SetOnRequireStop(Fx &&fx, Args &&...args)
        {
            _on_require_stop = std::bind(std::forward< Fx >(fx), std::forward< Args >(args)...);
            return *this;
        }

        template < class Fx, class... Args >
        Loop &SetOnStop(Fx &&fx, Args &&...args)
        {
            _on_stop = std::bind(std::forward< Fx >(fx), std::forward< Args >(args)...);
            return *this;
        }

    public:
        /**
         * @return 主循环的周期
         */
        poker::unit::Time GetPeriod() const;

        /**
         * @return 主循环的运行频率
         */
        double GetFrequency() const;

        /**
         * @return 是否正处于开始阶段
         */
        bool IsStarting() const;

        /**
         * @return 是否已经结束开始阶段，进入循环阶段
         */
        bool IsStarted() const;

        /**
         * @return 主循环是否正在运行
         */
        bool IsRunning() const;

        /**
         * @return 是否进入结束阶段
         */
        bool IsStopping() const;

        /**
         * @return 是否循环线程已经结束
         */
        bool IsStopped() const;

        /**
         * @return 调用本函数的线程是否与该循环线程处于同一个线程中
         */
        bool IsSameThread() const;

    protected:
        virtual void OnStart(Loop *self) = 0;

        virtual void OnRun(Loop *self) = 0;

        virtual void OnRequireStop(Loop *self) = 0;

        virtual void OnStop(Loop *self) = 0;

    private:
        /**
         * @brief 本循环线程主要流程
         */
        void Main();

        /**
         * @brief 结束主线程
         * @param wait 是否等待
         * @param callback 是否回调函数
         */
        void StopMainThread(bool wait, bool callback);

        /**
         * @brief 确保当前主线程已经完全结束
         */
        void JoinMainThread();

    private:
        // 生命周期回调函数
        std::function< void() > _on_start, _on_run, _on_require_stop, _on_stop;

        // 条件变量，用于控制等待流程
        poker::thread::Condition _is_started, _is_stopped, _is_resumed;

        // 状态变量，用于控制循环流程
        std::atomic_bool _is_paused = true, _is_stopping = false, _is_starting = false;

        // 暂停、恢复锁，避免异步冲突
        thread::MutexLock _pausing_resume_lock;

        // 主线程
        MutexLockable(std::thread) _main_thread;

        // 主线程周期
        MutexLockable(unit::Time) _period;
    };
}   // namespace poker::thread