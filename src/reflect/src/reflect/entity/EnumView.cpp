//
// Created by zyk on 24-6-3.
//

#include "poker/reflect/entity/EnumView.h"


namespace poker::reflect
{
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