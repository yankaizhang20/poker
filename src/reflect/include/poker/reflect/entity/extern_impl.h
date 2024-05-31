//
// Created by zyk on 24-5-31.
//

#pragma once


namespace poker::reflect
{
    class TypeView;

    /**
     * @brief 用户提供的反射函数
     */
    template < class T >
    void Bind(T &obj, TypeView &view);
}   // namespace poker::reflect