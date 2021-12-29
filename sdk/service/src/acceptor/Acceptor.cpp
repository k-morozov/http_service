//
// Created by focus on 12.12.2021.
//

#include "Acceptor.h"

#include "connection/include/Connection.h"

#include <boost/asio/executor.hpp>

namespace sdk {


Acceptor::Acceptor(boost::asio::io_context &context, boost::asio::ip::tcp::endpoint endpoint) :
        context_(context),
        acceptor_(context_),
        endpoint_(std::move(endpoint)),
        lg_("sdk", "Acceptor")
{
    lg_.info("create");
}

void Acceptor::prepare() {
    boost::system::error_code ec;
    acceptor_.open(endpoint_.protocol(), ec);
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint_, ec);

    acceptor_.listen(2, ec);
    lg_.info("start to listen");
}

struct Bar
{
    void operator()(boost::system::error_code ec, size_t bytes)
    {
        std::cout << "Hello from bar!" << std::endl;
    }

};
void Acceptor::run() {
    lg_.info("wait accept");
    acceptor_.async_accept(
            [this](boost::system::error_code code, boost::asio::ip::tcp::socket socket) {
                if (boost::asio::error::operation_aborted == code)
                    return;

                if (code)
                {
                    lg_.error_f("error connection %1%: %2%", code, code.message());
                    return;
                }

                lg_.info_f("connection from %1%", socket.remote_endpoint());

//                auto session = std::make_shared<sdk::Session>(
//                        context_,
//                        std::move(socket));
//
//                session->start();

                auto con = std::make_shared<Connection>(std::move(socket));

                Bar b;
                con->asyncRead(b);

                run();
            });
}

void Acceptor::stop() {

}

Acceptor::~Acceptor() {
    acceptor_.cancel();
    acceptor_.close();
    lg_.info("destroy");
}

}