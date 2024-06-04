//
// Created by zyk on 24-6-3.
//

#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "./extern_impl.h"


namespace poker::reflect
{
    /**
     * @brief 枚举值
     */
    using EnumNumber = long long;

    /**
     * @brief 描述一个枚举项
     */
    struct EnumItem
    {
        std::string name;
        EnumNumber  number = 0;

        template < class TEnum >
        EnumItem(std::string name, TEnum n) : name(std::move(name)), number(EnumNumber(n))
        {
        }
    };

    /**
     * @brief 枚举值与名称的查找表，一个名称只允许拥有一个枚举值，但多个名称可能对应同一个枚举值。
     */
    class EnumItemTable
    {
    public:
        EnumItemTable(std::initializer_list< EnumItem > list);

    public:
        /**
         * @return 全部的枚举项
         */
        [[nodiscard]]
        const std::vector< EnumItem > &List() const;

        /**
         * @brief 查询给定枚举值的全部枚举量名称，若不存在这样的索引设置，将返回空数组。
         */
        [[nodiscard]]
        const std::vector< std::string > &QueryNames(EnumNumber number) const;

        /**
         * @brief 查询给定枚举量名称对应的枚举值，若不存在这样的索引设置，将返回空
         */
        [[nodiscard]]
        std::optional< EnumNumber > QueryNumber(const std::string &name) const;

        [[nodiscard]]
        decltype(auto) begin() const
        {
            return items_.begin();
        };

        [[nodiscard]]
        decltype(auto) end() const
        {
            return items_.end();
        }

    private:
        // 全局的枚举项，由构造时的顺序确定
        std::vector< EnumItem > items_;

        // 全部的枚举项，由它们的名字索引
        std::map< std::string, EnumNumber > item_numbers_;

        // 相同枚举值可能对应多条枚举项，它们名称各有不同
        std::map< EnumNumber, std::vector< std::string > > item_names_;
    };

    class EnumView
    {
    public:
        template < class TEnum >
        explicit EnumView(TEnum &obj) : lut_ptr_(&Enumerating< TEnum >()), data_(&obj)
        {
            static_assert(std::is_enum_v< TEnum >);

            _number_setter = [ & ](EnumNumber number) { *(TEnum *) (data_) = TEnum(number); };

            _number_getter = [ & ]() { return EnumNumber(*(TEnum *) (data_)); };
        }

        explicit EnumView(const EnumItemTable &lut) : lut_ptr_(&lut)
        {
        }

    public:
        /**
         * @brief 设置枚举值
         */
        void SetNumber(EnumNumber n);

        /**
         * @return 获取枚举值
         */
        EnumNumber GetNumber() const;

        /**
         * @brief 尝试使用给定的枚举量名称进行枚举值设置
         * @return 是否设置成功
         */
        bool TrySetByName(const std::string &name);

        /**
         * @brief 根据当前的枚举值，返回拥有该值的全部枚举量名称
         */
        [[nodiscard]]
        const std::vector< std::string > &GetNames() const;

        /**
         * @return 本 enum 的所有条项
         */
        [[nodiscard]]
        const EnumItemTable &GetAllItems() const;

    private:
        std::function< void(EnumNumber number) > _number_setter;
        std::function< EnumNumber() >            _number_getter;

    private:
        const EnumItemTable *lut_ptr_ = nullptr;

        void *data_;
    };
}   // namespace poker::reflect