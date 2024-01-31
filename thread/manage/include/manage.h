#include <thread>

namespace poker::thread::manage
{
    /**
     * @brief 用以创建异常安全的线程
     */
    class ThreadAutoJoin
    {
    public:
        template < class F, class... Args >
        explicit ThreadAutoJoin(F &&f, Args &&...args)
        {
            main_thread_ = std::thread(std::forward< F >(f), std::forward< Args >(args)...);
        }

        ~ThreadAutoJoin()
        {
            if (main_thread_.joinable())
            {
                main_thread_.join();
            }
        }

    public:
        ThreadAutoJoin(const ThreadAutoJoin &another)             = delete;
        ThreadAutoJoin &operator= (const ThreadAutoJoin &another) = delete;

    private:
        std::thread main_thread_;
    };
}   // namespace poker::thread::manage
