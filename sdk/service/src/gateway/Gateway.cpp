//
// Created by focus on 12.12.2021.
//

#include <service/include/Gateway.h>
#include <connection/include/Connection.h>

#include <boost/asio/io_context.hpp>

#include <unordered_set>


namespace sdk {

Gateway::Gateway(boost::asio::io_context & io, protocol_t::endpoint ep) :
    context_(io),
    acceptor_(context_),
    endpoint_(std::move(ep)),
    lg_("sdk", "Gateway")
{
    lg_.info("create");
}

void Gateway::prepare()
{
    error_code ec;
    acceptor_.open(endpoint_.protocol(), ec);
    acceptor_.set_option(protocol_t::acceptor::reuse_address(true));
    acceptor_.bind(endpoint_, ec);

    acceptor_.listen(2, ec);
    lg_.info("start to listen");
}

void Gateway::run()
{
    lg_.info("wait accept");
    acceptor_.async_accept(
            [this](error_code ec, socket_t socket)
            {
                acceptHandler(ec, std::move(socket));
//                run();
            });
}

void Gateway::acceptHandler(error_code ec, socket_t socket)
{
    auto con = std::make_shared<Connection>(std::move(socket));
    con->asyncRead(ReadCompleter{});
    pool_.push_back(con);
}

void Gateway::stop() {
}

Gateway::~Gateway()
{
    acceptor_.cancel();
    acceptor_.close();
    lg_.info("destroy");
}


} // namespace sdk