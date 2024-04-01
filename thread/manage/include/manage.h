#include <thread>

namespace poker::thread::manage
{
    /**
     * @brief 用以创建异常安全的线程
     */
    class ThreadAutoJoin
    {
    public:
        /**
         * @brief 从头构造一个线程
         */
        template < class F, class... Args >
        explicit ThreadAutoJoin(F &&f, Args &&...args)
        {
            t_ = std::thread(std::forward< F >(f), std::forward< Args >(args)...);
        }

        /**
         * @brief 接管线程
         * @note 不对本类是否已经管理了线程进行检查
         */
        explicit ThreadAutoJoin(std::thread &&thread)
        {
            t_ = std::move(thread);
        }

        ~ThreadAutoJoin()
        {
            if (t_.joinable())
            {
                t_.join();
            }
        }

    private:
        std::thread t_;
    };
}   // namespace poker::thread::manage
