//
// Created by focus on 08.02.2022.
//

#pragma once

#include <memory>


namespace sdk {

class stop_state_base;
using stop_state_ptr = std::shared_ptr<stop_state_base>;


class stop_state_base
{
public:
    virtual ~stop_state_base() = 0;

    virtual void call() = 0;
    virtual void disposed() = 0;
};


stop_state_base::~stop_state_base() = default;


} // namespace sdk