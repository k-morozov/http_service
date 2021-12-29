//
// Created by focus on 28.12.2021.
//

#include "Impl.h"

namespace sdk {
    Connection::Impl::Impl(socket_t socket) :
            socket_(std::move(socket)),
            lg_("sdk", "Connection") {
        lg_.info("create");
    }

    Connection::Impl::~Impl() {
        lg_.info("destroy");
    }

    Connection::executor_type Connection::Impl::get_executor() {
        return socket_.get_executor();
    }
}