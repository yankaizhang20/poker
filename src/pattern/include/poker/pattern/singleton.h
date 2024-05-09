//
// Created by zyk on 24-5-9.
//

#pragma once


namespace poker::pattern
{
    /**
     * @brief 懒汉单例
     */
    template < class T >
    class GetGlobalUnique
    {
    public:
        static T &Ref()
        {
            static T data_;
            return data_;
        }

        static T *Ptr()
        {
            return &Ref();
        }
    };


    /**
     * @brief TODO: 真实单例
     */
    template < class T >
    class GlobalUnique
    {
    };
}   // namespace poker::pattern