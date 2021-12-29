//
// Created by focus on 28.12.2021.
//

#include "job_base.h"

namespace sdk {
    Connection::job_base::job_base(impl_ptr p) :
            self_(std::move(p)) {
    }

    const Connection::impl_ptr &Connection::job_base::self() const {
        return self_;
    }

    Connection::job_base::~job_base() = default;
}