//
// Created by focus on 28.12.2021.
//

#include "read_job_base.h"

Connection::read_job_base::read_job_base(Connection::impl_ptr p)
    : job_base(std::move(p))
{

}

void Connection::read_job_base::complete(Connection::lock_type &lck, Connection::error_code ec, size_t bytes)
{
    if (lck.owns_lock())
        lck.unlock();
    complete_impl(ec, bytes);

    delete this;
}

Connection::read_job_base::~read_job_base() = default;
