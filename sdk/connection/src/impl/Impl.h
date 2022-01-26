//
// Created by focus on 28.12.2021.
//

#pragma once

#include <connection/include/Connection.h>
#include <common/logger/Logger.h>

#include <boost/asio/ip/tcp.hpp>

#include <memory>
namespace sdk {

class Connection::Impl :
        std::enable_shared_from_this<Impl>
{
public:
    using socket_t = protocol_t::socket;

    explicit Impl(socket_t socket);

    ~Impl();

    executor_type get_executor();

    socket_t& get_socket() { return socket_; }

    void before_read_initiated(lock_type& lck, error_code& ec);

public:
    socket_t socket_;
    logger_mt lg_;
    mutable mutex_type mutex_;
};


}