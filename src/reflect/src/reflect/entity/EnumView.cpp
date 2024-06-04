//
// Created by zyk on 24-6-3.
//

#include <poker/macro.h>

#include "poker/reflect/entity/EnumView.h"


namespace poker::reflect
{
    EnumItemTable::EnumItemTable(std::initializer_list< EnumItem > list)
    {
        for (const EnumItem &item : list)
        {
            // 若找到相同名称的不同枚举项，则报错
            if (item_numbers_.find(item.name) != item_numbers_.end())
                poker_never("enumerating different enum item with the same name !");

            items_.push_back(item);
            item_numbers_[ item.name ] = item.number;
            item_names_[ item.number ].push_back(item.name);
        }
    }

    const std::vector< EnumItem > &EnumItemTable::List() const
    {
        return items_;
    }

    const std::vector< std::string > &EnumItemTable::QueryNames(EnumNumber number) const
    {
        static const std::vector< std::string > empty_names;

        if (auto it = item_names_.find(number); it != item_names_.end())
            return it->second;
        else
            return empty_names;
    }

    std::optional< EnumNumber > EnumItemTable::QueryNumber(const std::string &name) const
    {
        if (auto it = item_numbers_.find(name); it != item_numbers_.end())
            return it->second;
        else
            return {};
    }
}   // namespace poker::reflect


namespace poker::reflect
{
    void EnumView::SetNumber(EnumNumber n)
    {
        _number_setter(n);
    }

    EnumNumber EnumView::GetNumber() const
    {
        return _number_getter();
    }

    bool EnumView::TrySetByName(const std::string &name)
    {
        std::optional< EnumNumber > opt = lut_ptr_->QueryNumber(name);

        if (opt.has_value())
        {
            SetNumber(opt.value());
            return true;
        }

        return false;
    }

    const std::vector< std::string > &EnumView::GetNames() const
    {
        return lut_ptr_->QueryNames(GetNumber());
    }

    const EnumItemTable &EnumView::GetAllItems() const
    {
        return *lut_ptr_;
    }
}   // namespace poker::reflect