//
// Created by focus on 28.12.2021.
//

#pragma once

#include <connection/include/Connection.h>

//#include <boost/beast/core/flat_buffer.hpp>
#include <boost/asio/buffer.hpp>

#include "job_base.h"

namespace sdk {

//    struct Connection::read_job_base :
//            public job_base
//    {
//    public:
////        using Buffer = boost::asio::buffer;
//
//        explicit read_job_base(impl_ptr p);
//
//        ~read_job_base() override;
//
//        auto& get_buffer() noexcept { return data_; }
//
//        void complete(lock_type &lck, error_code code, size_t bytes);
//
//    private:
//        virtual void complete_impl(error_code code, size_t bytes) = 0;
//
//    private:
//        std::array<uint8_t, 8128> data_;
//    };
}