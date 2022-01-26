//
// Created by focus on 12.12.2021.
//

#ifndef ANALYTICS_GATEWAY_H
#define ANALYTICS_GATEWAY_H

#include "IService.h"

#include <common/logger/Logger.h>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>

#include <memory>


namespace sdk {


class Connection;

class Gateway final : public IService {
public:
    using error_code = boost::system::error_code;
    using protocol_t = boost::asio::ip::tcp;
    using socket_t = protocol_t::socket;

    explicit Gateway(boost::asio::io_context & io, protocol_t::endpoint ep);

    void prepare() override;

    void run() override;

    void stop() override;

    ~Gateway() override;

private:
    boost::asio::io_context& context_;
    protocol_t::acceptor acceptor_;
    protocol_t::endpoint endpoint_;

    logger_t lg_;

    std::vector<std::shared_ptr<Connection>> pool_;


    void acceptHandler(error_code ec, socket_t socket);


    struct ReadCompleter
    {
//        explicit ReadCompleter() = default;
        void operator()(boost::system::error_code ec, size_t bytes)
        {
            std::cout << "read_job: " << ec.message() << ", bytes=" << bytes << std::endl;
        }
    private:

    };
};


}

#endif //ANALYTICS_GATEWAY_H
