//
// Created by zyk on 24-6-4.
//

#pragma once

#include <functional>
#include <string>


namespace poker::databus
{
    template < class Message >
    using Handler = std::function< void(Message) >;

    template < class Request, class Response >
    using Server = std::function< bool(const Request &, Response &) >;

    template < class T >
    using Decoder = std::function< std::optional< T >(const std::string &) >;
}   // namespace poker::databus