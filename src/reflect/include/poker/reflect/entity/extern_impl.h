//
// Created by zyk on 24-5-31.
//

#pragma once


namespace poker::reflect
{
    class TypeView;
    class EnumItemTable;

    /**
     * @brief 用户提供的反射函数
     */
    template < class T >
    void Bind(T &obj, TypeView &view);

    /**
     * @brief 指定枚举类型的枚举项查找表，可获得用户为某个枚举类型注册的
     * 可选项及其名称，用于可能的可视化模块使用。
     */
    template < class TEnum >
    const EnumItemTable &Enumerating();
}   // namespace poker::reflect