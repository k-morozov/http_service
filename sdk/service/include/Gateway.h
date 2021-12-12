//
// Created by focus on 12.12.2021.
//

#ifndef ANALYTICS_GATEWAY_H
#define ANALYTICS_GATEWAY_H

#include "IService.h"

#include <common/logger/Logger.h>

#include <memory>


class Gateway final : public IService {
public:
    Gateway();

    void prepare() override;

    void run() override;

    void stop() override;

    ~Gateway() override;

private:
    class GatewayImpl;
    std::unique_ptr<GatewayImpl> impl_;

    logger_t lg_;
};



#endif //ANALYTICS_GATEWAY_H
