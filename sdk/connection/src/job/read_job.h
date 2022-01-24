//
// Created by focus on 28.12.2021.
//

#pragma once

#include <boost/beast/core/async_base.hpp>

#include <connection/include/Connection.h>
#include <connection/src/impl/Impl.h>

#include <boost/asio/executor.hpp>

#include "read_job_base.h"

namespace sdk {

//    template<class THandler>
//    struct Connection::read_job final
//            : public read_job_base {
//        using handler_t = boost::beast::async_base<THandler, executor_type>;
//
//    public:
//        template<class U>
//        read_job(U &&h, impl_ptr self);
//
//        ~read_job() override = default;
//
//    private:
//        handler_t handler_;
//
//    private:
//        void complete_impl(error_code ec, size_t bytes) override {
//            handler_.template complete_now(ec, bytes);
//        }
//    };
//
//
//    template<class THandler>
//    template<class U>
//    Connection::read_job<THandler>::read_job(U &&h, Connection::impl_ptr self) :
//        read_job_base(self),
//        handler_(std::forward<U>(h), self->get_executor())
//    {
//
//    }

}