//
// Created by zyk on 24-6-5.
//

#include <thread>

#include <poker/chrono.h>
#include <poker/system.h>
#include <poker/thread/lock.h>

#include "poker/thread/Loop.h"


namespace poker::thread
{
    void Loop::Start()
    {
        // 启动新的主循环时，先结束旧循环
        Stop(true);

        // 确保主线程从系统上消失
        JoinMainThread();

        // 初始化状态变量
        _is_starting = true;
        _is_paused   = false;
        _is_stopping = false;

        // 创建主线程
        Locking(_main_thread)
        {
            _main_thread = std::thread(&Loop::Main, this);
        }
    }

    void Loop::Pause()
    {
        Locking(_pausing_resume_lock)
        {
            _is_resumed.Init(false);
            _is_paused = true;
        }
    }

    void Loop::Resume()
    {
        Locking(_pausing_resume_lock)
        {
            _is_resumed.Satisfy();
            _is_paused = false;
        }
    }

    void Loop::Stop(bool wait)
    {
        // 主动结束本循环线程，将进行函数回调（属正常流程）
        StopMainThread(wait, true);
    }

    void Loop::Wait()
    {
        if (IsSameThread())
            return;

        _is_stopped.Wait();
    }

    bool Loop::Wait(const unit::Time &time)
    {
        if (IsSameThread())
            return false;

        return _is_stopped.Wait(time);
    }

    void Loop::WaitStarted()
    {
        if (IsSameThread())
            return;

        _is_started.Wait();
    }

    bool Loop::WaitStarted(const unit::Time &time)
    {
        if (IsSameThread())
            return false;

        return _is_started.Wait(time);
    }

    Loop::~Loop()
    {
        // 析构时结束主线程，不可调用回调函数，因为此时子类已经不存在。
        StopMainThread(true, false);

        // 确保主线程从系统上消失
        JoinMainThread();
    }

    Loop &Loop::SetFrequency(double hz)
    {
        Locking(_period) _period = poker::unit::Second(1.0 / hz);
        return *this;
    }

    Loop &Loop::SetPeriod(const unit::Time &time)
    {
        Locking(_period) _period = time;
        return *this;
    }

    Loop &Loop::SetAsFastAsPossible()
    {
        Locking(_period) _period = poker::unit::Second(0);
        return *this;
    }

    poker::unit::Time Loop::GetPeriod() const
    {
        Locking(_period) return (poker::unit::Time &) _period;
    }

    double Loop::GetFrequency() const
    {
        Locking(_period) return 1.0 / _period.Get< poker::unit::Second >();
    }

    bool Loop::IsStarting() const
    {
        return _is_starting;
    }

    bool Loop::IsStarted() const
    {
        return _is_started;
    }

    bool Loop::IsRunning() const
    {
        return not _is_stopped and not _is_paused and _is_started;
    }

    bool Loop::IsStopping() const
    {
        return _is_stopping;
    }

    bool Loop::IsStopped() const
    {
        return _is_stopped;
    }

    bool Loop::IsSameThread() const
    {
        Locking(_main_thread)
        {
            if (not _main_thread.joinable())
                return false;

            return _main_thread.get_id() == std::this_thread::get_id();
        }
    }

    void Loop::Main()
    {
        // 进入开始阶段
        OnStart(this);

        if (_on_start)
            _on_start();

        // 结束开始阶段
        _is_starting = false;
        _is_started.Satisfy();

        // 主循环
        while (poker::system::Ok())
        {
            // 判断主循环是否被暂停，是则等待暂停解除；
            // 若解除时条件返回 false，说明主循环在暂停时被要求关闭
            if (_is_paused)
            {
                if (not _is_resumed.Wait())
                    break;
            }

            // 判断当前是否被要求退出主循环
            if (_is_stopping)
                break;

            // 记录本次处理周期的开始时间，将用于计算最终的休眠时间
            auto t1 = poker::chrono::now();

            // 执行主循环的处理函数
            OnRun(this);

            if (_on_run)
                _on_run();

            // 记录处理后的时间，并计算当前周期的休眠时间，进行休眠
            auto t2 = poker::chrono::now();

            poker::system::Sleep(_period - (t2 - t1));
        }

        // 退出主循环，进入结束阶段
        OnStop(this);

        if (_on_stop)
            _on_stop();

        // 循环线程完全结束
        _is_stopped.Satisfy();
        _is_stopping = false;
    }

    void Loop::StopMainThread(bool wait, bool callback)
    {
        if (_is_stopped)
        {
            return;
        }

        // 若线程还没有退出，则标识线程正在退出：控制主线程退出
        _is_stopping = true;

        // 若线程处于暂停状态，则需要解除该状态（并使它的恢复条件返回 false）
        if (_is_paused)
            _is_resumed.Fail();

        if (callback)
        {
            OnRequireStop(this);

            if (_on_require_stop)
                _on_require_stop();
        }

        if (wait)
        {
            Wait();
        }
    }

    void Loop::JoinMainThread()
    {
        Locking(_main_thread)
        {
            if (_main_thread.joinable())
                _main_thread.join();
        }

        // 该函数不可以在 Stop() 函数中调用，因为 Stop() 函数可能会在本循环线程的生命周期函数中被调用，
        // 若调用本函数，则将产生自己 join 自己的情况，导致死锁或者其他 bug.
    }
}   // namespace poker::thread