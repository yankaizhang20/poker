//
// Created by zyk on 24-6-3.
//

#pragma once

#include <string>


namespace poker::param
{
    /**
     * @brief 读取指定 id 的参数
     */
    template < class T >
    bool Read(const std::string &id, T &param);

    /**
     * @brief 写入指定 id 的参数
     */
    template < class T >
    bool Write(const std::string &id, const T &param);
}   // namespace poker::param