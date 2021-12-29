//
// Created by focus on 12.12.2021.
//

#ifndef ANALYTICS_ACCEPTOR_H
#define ANALYTICS_ACCEPTOR_H

#include <common/logger/Logger.h>
#include <service/include/IService.h>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>


namespace sdk {

class Acceptor final : public IService {
public:

    explicit Acceptor(boost::asio::io_context &, boost::asio::ip::tcp::endpoint endpoint);

    void prepare() override;

    void run() override;

    void stop() override;

    ~Acceptor() override;

private:
    boost::asio::io_context& context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::endpoint endpoint_;

    logger_t lg_;
};

}


#endif //ANALYTICS_ACCEPTOR_H
