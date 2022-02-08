//
// Created by focus on 08.02.2022.
//

#pragma once

#include <common/logger/Logger.h>

#include "stop_state_base.h"

#include <thread>


namespace sdk {


class stop_state_registry
{
public:
    using mutex_t = std::mutex;
    using lock_t = std::unique_lock<mutex_t>;


    static stop_state_registry& instance()
    {
        static stop_state_registry obj;
        return obj;
    }

    void push(stop_state_ptr p);

    void pop() {}
    void notify() {}

private:
    stop_state_registry()
    {
        worker_.emplace([this](){ do_poll(); });
    }

    void do_poll() {};

    void notify(lock_t& lck, stop_state_ptr p);

private:
    logger_mt lg_{"stop_state_impl", this};
    mutable mutex_t mutex_;
    bool disposed_ = false;
    std::optional<std::thread> worker_;

    using container_type = std::vector<stop_state_ptr>;
    container_type pending_;
    container_type triggered_;
};


} // namespace sdk
