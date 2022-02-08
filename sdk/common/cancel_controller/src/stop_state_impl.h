//
// Created by focus on 08.02.2022.
//

#pragma once

#include "stop_state_base.h"

#include <functional>


namespace sdk {


class stop_state_impl
        : public stop_state_base
{
public:
    using stop_action = std::function<void()>;

    stop_state_impl() = default;
    ~stop_state_impl() override = default;

    void request_stop();
    void connect();

};


} // namespace sdk