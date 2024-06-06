//
// Created by zyk on 24-3-14.
//

#pragma once

#include <functional>
#include <map>
#include <string>


namespace poker::databus::http
{
    class IParamRequest
    {
    public:
        using Visitor = std::function< void(std::string name, std::string value) >;

    public:
        virtual void Visit(const Visitor &visitor) const = 0;
    };   // namespace poker::databus::http


    class ParamRequest : public IParamRequest
    {
    public:
        std::string &operator[] (const std::string &key)
        {
            return _params[ key ];
        }

    public:
        void Visit(const Visitor &visitor) const override
        {
            for (const auto &pair : _params)
            {
                visitor(pair.first, pair.second);
            }
        }

    private:
        std::unordered_map< std::string, std::string > _params;
    };


}   // namespace poker::databus::http