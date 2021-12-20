//
// Created by focus on 12.12.2021.
//

#include <service/include/Gateway.h>

#include <boost/asio/io_context.hpp>

#include "acceptor/Acceptor.h"

#include <unordered_set>


namespace sdk {

class Gateway::GatewayImpl final : public IService {
public:

    explicit GatewayImpl(std::unique_ptr<sdk::Acceptor> acceptor) :
        acceptor_(std::move(acceptor)),
        lg_("sdk", "Gateway")
    {
        if (!acceptor_)
            throw std::bad_alloc();

        lg_.info("create");
    }

    void prepare() override {
        lg_.info("prepare");
        acceptor_->prepare();
    }

    void run() override {
        acceptor_->run();
    }

    void stop() override {
        acceptor_->stop();
    }

    ~GatewayImpl() override {
        lg_.info("destroy");
    }

private:
    std::unique_ptr<sdk::Acceptor> acceptor_;
    logger_t lg_;
};

Gateway::Gateway(std::unique_ptr<sdk::Acceptor> acceptor)
{
    impl_ = std::make_unique<GatewayImpl>(std::move(acceptor));
    if (!impl_)
        throw std::bad_alloc();
}

void Gateway::prepare() {
    impl_->prepare();
}

void Gateway::run() {
    impl_->run();
}

void Gateway::stop() {
    impl_->stop();
}

Gateway::~Gateway() {
//    lg_.info("destroy");
}


}