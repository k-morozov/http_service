//
// Created by focus on 28.12.2021.
//

#pragma once


#include <boost/asio/ip/tcp.hpp>

#include <memory>


class Connection
{
public:
    using protocol_t = boost::asio::ip::tcp;


    explicit Connection(protocol_t::socket socket);
    ~Connection();

private:
    class Impl;
    using ImplPtr = std::shared_ptr<Impl>;

    ImplPtr impl_;

};
