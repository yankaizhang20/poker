#include <thread>

namespace poker::thread
{
    enum class Retrieve
    {
        Join,
        Detach
    };

    /**
     * @brief 用以创建异常安全的线程
     */
    class Thread
    {
    public:
        /**
         * @brief 从头构造一个线程
         */
        template < class F, class... Args >
        explicit Thread(F &&f, Args &&...args, Retrieve retrieve_action) :
            retrieve_action_(retrieve_action), t_(std::thread(std::forward< F >(f), std::forward< Args >(args)...))
        {
        }

        /**
         * @brief 接管线程
         * @note 不对本类是否已经管理了线程进行检查
         */
        explicit Thread(std::thread &&thread, Retrieve retrieve_action) :
            retrieve_action_(retrieve_action), t_(std::move(thread))
        {
        }

        ~Thread()
        {
            if (t_.joinable())
            {
                if (retrieve_action_ == Retrieve::Join)
                {
                    t_.join();
                }
                else
                {
                    t_.detach();
                }
            }
        }

        std::thread &Get()
        {
            return t_;
        }

    private:
        Retrieve retrieve_action_;

        std::thread t_;
    };

    void Test();
}   // namespace poker::thread
