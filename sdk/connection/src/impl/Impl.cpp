//
// Created by focus on 28.12.2021.
//

#include "Impl.h"

Connection::Impl::Impl(socket_t socket) :
    socket_(std::move(socket)),
    lg_("sdk", "Connection")
{
    lg_.info("create");
}

Connection::Impl::~Impl()
{
    lg_.info("destroy");
}
