//
// Created by focus on 12.12.2021.
//

#ifndef ANALYTICS_GATEWAY_H
#define ANALYTICS_GATEWAY_H

#include "IService.h"

#include <common/logger/Logger.h>

#include <memory>


namespace sdk {


class Acceptor;

class Gateway final : public IService {
public:
    explicit Gateway(std::unique_ptr<sdk::Acceptor> acceptor);

    void prepare() override;

    void run() override;

    void stop() override;

    ~Gateway() override;

private:
    class GatewayImpl;
    std::unique_ptr<GatewayImpl> impl_;
};


}

#endif //ANALYTICS_GATEWAY_H
