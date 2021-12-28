//
// Created by focus on 28.12.2021.
//

#include <connection/include/Connection.h>

#include "impl/Impl.h"

Connection::Connection(boost::asio::ip::tcp::socket socket)
{
}

Connection::~Connection()
{

}

Connection::executor_type Connection::get_executor()
{
    return impl_->get_executor();
}