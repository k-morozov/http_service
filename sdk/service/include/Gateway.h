//
// Created by focus on 12.12.2021.
//

#ifndef ANALYTICS_GATEWAY_H
#define ANALYTICS_GATEWAY_H

#include "IService.h"

#include <common/logger/Logger.h>
#include <service/src/core/Core.h>

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

using core_ptr = std::unique_ptr<Core>;

class Gateway final : public IService {
public:
    using error_code = boost::system::error_code;
    using protocol_t = boost::asio::ip::tcp;
    using socket_t = protocol_t::socket;
    using acceptor_t = protocol_t::acceptor;
    using endpoint_t = protocol_t::endpoint;

    explicit Gateway(boost::asio::io_context & io, endpoint_t const& ep);

    void setup_pipeline(PipelinePtr&& pipeline)
    {
        core_->setup_pipeline(std::move(pipeline));
    }

    void run() override;

    void stop() override;

    ~Gateway() override
    {
        acceptor_->cancel();
        acceptor_->close();
        acceptor_.reset();

        lg_.info("destroy");
    }

private:
    core_ptr core_;
    std::unique_ptr<acceptor_t> acceptor_;
    logger_t lg_;

    boost::asio::coroutine coro_;
    std::vector<std::shared_ptr<Connection>> pool_;


    struct TransactFinished
    {
        explicit TransactFinished(logger_t& lg) :
            lg_(lg)
        {};
        void operator()(error_code ec)
        {
            lg_.info_f("transact completed: %1%", ec.message());
        }
        logger_t& lg_;
    };
};


}

#endif //ANALYTICS_GATEWAY_H
