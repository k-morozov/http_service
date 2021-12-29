//
// Created by focus on 28.12.2021.
//

#pragma once

#include <connection/include/Connection.h>

namespace sdk {
    struct Connection::job_base {
    public:
        explicit job_base(impl_ptr p);

        virtual ~job_base();

        job_base() = delete;

        job_base(job_base const &) = delete;

        job_base(job_base &&) = delete;

        job_base &operator=(job_base const &) = delete;

        job_base &operator=(job_base &&) = delete;

        [[nodiscard]]
        impl_ptr const &self() const;

    protected:
        impl_ptr self_;
    };
}