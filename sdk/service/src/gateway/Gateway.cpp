//
// Created by focus on 12.12.2021.
//

#include <service/include/Gateway.h>
#include <connection/include/Connection.h>


#include <boost/asio/io_context.hpp>
#include <boost/asio/yield.hpp>

#include <unordered_set>


namespace sdk {

Gateway::Gateway(boost::asio::io_context & io, endpoint_t const& ep) :
    lg_("sdk", "Gateway")
{
    core_ = std::make_unique<Core>(io);
    if (!core_)
        throw std::invalid_argument("failed create core");

    // @TODO pipeline in new class?
    auto print = [](Connection::request_t const& req) -> Core::error_code
            { std::cout << req << std::endl; return {}; };
    core_->add_request_action(std::move(print));

    acceptor_ = std::make_unique<acceptor_t>(core_->get_context());
    if (!acceptor_)
        throw std::invalid_argument("failed create acceptor");

    error_code ec;
    acceptor_->open(ep.protocol(), ec);
    if (ec)
        throw std::invalid_argument("failed open acceptor");

    acceptor_->set_option(protocol_t::acceptor::reuse_address(true));
    acceptor_->bind(ep, ec);
    if (ec)
        throw std::invalid_argument("failed bind to endpoint");

    acceptor_->listen(2, ec);
    if (ec)
        throw std::invalid_argument("failed listen to endpoint");

    lg_.info("create");
}

void Gateway::prepare()
{

}

void Gateway::run()
{
    // @TODO add cancel, counter active transaction

    reenter(coro_)
    {
        for(;;)
        {
            lg_.info("wait accept");
            yield acceptor_->async_accept(
                [this](error_code ec, socket_t socket)  mutable
                {
                    lg_.info_f("accept completed, ec=%1%, message=%2%", ec, ec.message());
                    if (!ec)
                    {
                        core_->async_transact(std::move(socket), TransactFinished{lg_});
                    }
                    run();
                });
        }
    }

    if (coro_.is_complete())
    {
        lg_.info("coro complete");
    }
}

void Gateway::stop() {
}


} // namespace sdk