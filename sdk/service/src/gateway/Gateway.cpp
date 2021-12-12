//
// Created by focus on 12.12.2021.
//

#include <service/include/Gateway.h>
#include "acceptor/Acceptor.h"

#include <unordered_set>


class Gateway::GatewayImpl final : public IService {
public:
    void prepare() override {

    }

    void run() override {

    }

    void stop() override {

    }

    ~GatewayImpl() override {

    }

private:
};


Gateway::Gateway() :
    lg_("sdk", "Gateway")
{
    lg_.info("create");
}


void Gateway::prepare()
{
}

void Gateway::run()
{

}

void Gateway::stop()
{

}

Gateway::~Gateway()
{
    lg_.info("destroy");
}
