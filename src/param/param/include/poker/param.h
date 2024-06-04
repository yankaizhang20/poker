//
// Created by zyk on 24-6-3.
//

#pragma once

#include <string>

#include <poker/pattern.h>
#include <poker/reflect.h>

#include "./param/module/Storage.h"


namespace poker::param
{
    namespace backend
    {
        /**
         * @brief 设置参数后端的读取器
         */
        void SetReader(Storage::Reader reader);

        /**
         * @brief 设置参数后端的写入器
         */
        void SetWriter(Storage::Writer writer);

        /**
         * @brief 卸载参数后端读取器
         */
        void UnsetReader();

        /**
         * @brief 卸载参数后端写入器
         */
        void UnsetWriter();
    }   // namespace backend

    /**
     * @brief 读取指定 id 的参数
     */
    template < class T >
    bool Read(const std::string &id, T &param)
    {
        // step 1: 创建与给定变量绑定的参数对象
        reflect::TypeView param_view;
        param_view.Bind(param);

        // step 2: 读取本地参数
        return pattern::GetGlobalUnique< Storage >::Ref().Read(id, param_view);
    }

    /**
     * @brief 写入指定 id 的参数
     */
    template < class T >
    bool Write(const std::string &id, const T &param)
    {
        // step 1: 创建与给定变量绑定的参数对象
        reflect::TypeView param_view;
        param_view.Bind(const_cast< T & >(param));

        // step 2: 写入到本地参数
        return pattern::GetGlobalUnique< Storage >::Ref().Write(id, param_view);
    }
}   // namespace poker::param