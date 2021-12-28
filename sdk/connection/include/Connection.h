//
// Created by focus on 28.12.2021.
//

#pragma once


#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/executor.hpp>

#include <memory>
#include <iostream>
#include <mutex>


class Connection
{
public:
    using protocol_t = boost::asio::ip::tcp;
    using error_code = boost::system::error_code;
    using executor_type = boost::asio::executor;
    using mutex_type = std::mutex;
    using lock_type = std::unique_lock<mutex_type>;


    explicit Connection(protocol_t::socket socket);
    ~Connection();

    executor_type get_executor();

    struct foo
    {
        template<class T>
        void operator()(T&& h) const
        {
            std::cout << "Hello from foo" << std::endl;
            h();
        }
    };

    template<class THandler>
    BOOST_ASIO_INITFN_RESULT_TYPE(THandler, void(void)) asyncRead(THandler&& h)
    {
        return boost::asio::async_initiate<THandler, void(void)>(foo{}, h);
    }

private:
    class Impl;
    using impl_ptr = std::shared_ptr<Impl>;

    impl_ptr impl_;


private:

    struct run_read
    {
        template<typename THandler>
        void operator()(THandler&& h, Connection* self, lock_type& lck)
        {
            BOOST_ASIO_READ_HANDLER_CHECK(THandler, h) type_check;

            using job_t = read_job<typename std::decay_t<THandler>>;

        }
    };

private:
    struct job_base;
    struct read_job_base;
    template<class T>
    struct read_job;
};
