//
// Created by focus on 28.12.2021.
//

#include "Impl.h"

Connection::Impl::Impl(boost::asio::ip::tcp::socket socket) :
    socket_(std::move(socket)),
    lg_("sdk", "Connection")
{
    lg_.info("create");
}

Connection::Impl::~Impl()
{
    lg_.info("destroy");
}
