//
// Created by focus on 28.12.2021.
//

#include <connection/include/Connection.h>

#include <boost/asio.hpp>

#include "impl/Impl.h"
#include "job/read_job_base.h"


namespace sdk {


Connection::Connection(boost::asio::ip::tcp::socket socket) :
    impl_(std::make_shared<Impl>(std::move(socket)))
{
}

Connection::~Connection() = default;

Connection::executor_type Connection::get_executor()
{
    return impl_->get_executor();
}

Connection::mutex_type& Connection::mutex()
{
    return impl_->mutex_;
}

Connection::impl_ptr Connection::get_impl()
{
    return impl_;
}

void Connection::initiate_read(lock_type& lck, read_job_base* job)
{
    error_code ec;
    impl_->before_read_initiated(lck, ec);

    if (!ec)
    {
        initiate_read_impl(lck, job);
    }
    else
    {
        boost::asio::post(get_executor(),
                          [job, ec]()
                          {
                            lock_type stub; // @TODO fix stub
                            job->complete(stub, ec, 0u);
                          });
    }
}

void Connection::initiate_read_impl(lock_type& lck, read_job_base* job)
{
    impl_->socket_.async_read_some(boost::asio::buffer(job->get_buffer()),
                                   [job](error_code ec, size_t bytes)
                                   {
                                        auto const& self = job->self();
                                        lock_type lck(self->mutex_);
                                        job->complete(lck, ec, bytes);
                                   });
}

Connection::job_base::job_base(impl_ptr p) :
        self_(std::move(p))
{
}

const Connection::impl_ptr &Connection::job_base::self() const
{
    return self_;
}

Connection::job_base::~job_base() = default;

Connection::read_job_base::read_job_base(Connection::impl_ptr p)
    : job_base(std::move(p))
{
}

void Connection::read_job_base::complete(Connection::lock_type &lck, Connection::error_code ec, size_t bytes) {
    if (lck.owns_lock())
        lck.unlock();
    complete_impl(ec, bytes);

    delete this;
}

Connection::read_job_base::~read_job_base() = default;


}