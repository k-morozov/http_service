//
// Created by focus on 28.12.2021.
//

#pragma once

#include <connection/include/Connection.h>
#include <common/logger/Logger.h>

#include <boost/asio/ip/tcp.hpp>

#include <memory>

class Connection::Impl :
    std::enable_shared_from_this<Impl>
{
public:
    using socket_t = protocol_t::socket;

    explicit Impl(socket_t socket);
    ~Impl();



private:
    socket_t socket_;
    logger_mt lg_;
};


