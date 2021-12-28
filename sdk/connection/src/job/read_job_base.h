//
// Created by focus on 28.12.2021.
//

#pragma once

#include <connection/include/Connection.h>

#include "job_base.h"


struct Connection::read_job_base :
        public job_base
{
public:
    explicit read_job_base(impl_ptr p);
    ~read_job_base() override;

    void complete(lock_type& lck, error_code code, size_t bytes);

private:
    virtual void complete_impl(error_code code, size_t bytes) = 0;

private:
    // message, buffers, e.t.c.
};
