//
// Created by focus on 12.12.2021.
//

#include "Acceptor.h"



Acceptor::Acceptor(boost::asio::io_context& context, boost::asio::ip::tcp::endpoint endpoint) :
    context_(context),
    acceptor_(boost::asio::make_strand(context_)),
    endpoint_(std::move(endpoint)),
    lg_("sdk", "Acceptor")
{
    lg_.info("create");
}

void Acceptor::prepare()
{
    boost::system::error_code ec;
    acceptor_.open(endpoint_.protocol(), ec);
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint_, ec);

    acceptor_.listen(2, ec);
    lg_.info("start to listen");
}

void Acceptor::run()
{
    lg_.info("wait accept");
    acceptor_.async_accept(
            boost::asio::make_strand(context_),
            [this](auto&& arg1, auto&& arg2) {
                std::stringstream ss;
                ss << "new connection, thread_id=" << std::this_thread::get_id();
                lg_.info(ss.str());

                run();
            });
}

void Acceptor::stop()
{

}

Acceptor::~Acceptor()
{
    lg_.info("destroy");
}
