//
// Created by focus on 12.12.2021.
//

#include "Acceptor.h"

namespace sdk {


Acceptor::Acceptor(boost::asio::io_context &context, boost::asio::ip::tcp::endpoint endpoint) :
        context_(context),
        acceptor_(boost::asio::make_strand(context_)),
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

void Acceptor::run() {
    lg_.info("wait accept");
    acceptor_.async_accept(
            [this](boost::system::error_code code, auto &&arg2) {
                if (boost::asio::error::operation_aborted == code)
                    return;

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