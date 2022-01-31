//
// Created by focus on 28.12.2021.
//

#include "Impl.h"

#include <boost/system/error_code.hpp>


namespace sdk {


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

Connection::executor_type Connection::Impl::get_executor()
{
    return socket_.get_executor();
}

void Connection::Impl::before_read_initiated(lock_type& lck, error_code& ec)
{
    if (!ec && !lck.owns_lock())
    {
        // @TODO add new error type
        ec = boost::system::errc::make_error_code(boost::system::errc::timed_out);
        lg_.error("Mutex should be lock for read");
    }
}

void Connection::Impl::before_write_initiated(lock_type& lck, error_code& ec)
{
    if (!ec && !lck.owns_lock())
    {
        // @TODO add new error type
        ec = boost::system::errc::make_error_code(boost::system::errc::timed_out);
        lg_.error("Mutex should be lock for write");
    }
}

}