//
// Created by zyk on 24-6-3.
//

#pragma once

#include <functional>

#include <poker/reflect.h>


namespace poker::param
{
    class Storage
    {
    public:
        /**
         * @brief 参数读取器：读取指定 id 的参数，返回是否读取成功。
         */
        using Reader = std::function< bool(const std::string &id, reflect::TypeView &parameter) >;

        /**
         * @brief 参数写入器：写入指定 id 的参数，返回是否写入成功。
         */
        using Writer = std::function< bool(const std::string &id, const reflect::TypeView &parameter) >;

    public:
        void SetReader(Reader reader);

        void SetWriter(Writer writer);

        bool Read(const std::string &id, reflect::TypeView &parameter);

        bool Write(const std::string &id, const reflect::TypeView &parameter);

    private:
        Reader _reader;
        Writer _writer;
    };
}   // namespace poker::param
