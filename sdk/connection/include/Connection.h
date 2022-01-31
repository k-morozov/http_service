//
// Created by focus on 28.12.2021.
//

#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/beast/core/async_base.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/version.hpp>

#if BOOST_VERSION >= 107400
    #include <boost/asio/executor.hpp>
#else
    #include <boost/asio/any_io_executor.hpp>
#endif

#include <memory>
#include <iostream>
#include <mutex>


namespace sdk {


class Connection
{
public:
    using protocol_t = boost::asio::ip::tcp;
    using endpoint_t = typename protocol_t ::endpoint ;
    using error_code = boost::system::error_code;
#if BOOST_VERSION >= 107400
        using executor_type = boost::asio::any_io_executor;
#else
        using executor_type = boost::asio::executor;
#endif
    using mutex_type = std::mutex;
    using lock_type = std::unique_lock<mutex_type>;
    using string_body_t = boost::beast::http::string_body;
    using empty_body_t = boost::beast::http::empty_body;
    using request_t = boost::beast::http::request<string_body_t>;
    using response_t = boost::beast::http::response<empty_body_t>;


    explicit Connection(protocol_t::socket socket);
    ~Connection();

    executor_type get_executor();
    mutex_type& mutex();

    [[nodiscard]]
    request_t const& request() const
    {
        // @TODO add parse?
        return req_;
    };

    void set_response(response_t const& _)
    {
        namespace http = boost::beast::http;
        response_t res{http::status::ok, req_.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
//        res.set(http::field::content_type, mime_type(path));
//        res.content_length(size);
        res.keep_alive(req_.keep_alive());
        res_ = res;
    }

    template<class THandler>
    BOOST_ASIO_INITFN_RESULT_TYPE(THandler, void(error_code, size_t)) async_read(THandler&& h);

    template<class THandler>
    BOOST_ASIO_INITFN_RESULT_TYPE(THandler, void(error_code, size_t)) async_write(THandler&& h);

private:
    class Impl;
    using impl_ptr = std::shared_ptr<Impl>;
    impl_ptr impl_;

    // @TODO state - init, read, pending, write, complete
    request_t req_;
    response_t res_;

public:
    impl_ptr get_impl();

private:
    struct job_base
    {
    public:
        explicit job_base(impl_ptr p);
        virtual ~job_base();

        job_base() = delete;
        job_base(job_base const &) = delete;
        job_base(job_base &&) = delete;
        job_base &operator=(job_base const &) = delete;
        job_base &operator=(job_base &&) = delete;

        [[nodiscard]]
        impl_ptr const& self() const;

    protected:
        impl_ptr self_;
    };

    struct read_job_base : public job_base
    {
    public:
        explicit read_job_base(impl_ptr p);
        ~read_job_base() override;

        auto& get_buffer() noexcept { return data_; }
        void complete(lock_type &lck, error_code code, size_t bytes);
    private:
        boost::beast::flat_buffer data_{8128};

        virtual void complete_impl(error_code code, size_t bytes) = 0;
    };

    struct write_job_base : public job_base
    {
    public:
        explicit write_job_base(impl_ptr p);
        ~write_job_base() override;

//        auto& get_buffer() noexcept { return data_; }
        void complete(lock_type &lck, error_code code, size_t bytes);
    private:
//        boost::beast::flat_buffer data_{8128};

        virtual void complete_impl(error_code code, size_t bytes) = 0;
    };

    template<class THandler>
    struct read_job final : public read_job_base
    {
        using handler_t = boost::beast::async_base<THandler, executor_type>;
    public:
        template<class U>
        read_job(U&& h, Connection* self) :
                read_job_base(self->get_impl()),
                handler_(std::forward<U>(h), self->get_executor())
        {}
        ~read_job() override = default;

    private:
        handler_t handler_;

        void complete_impl(error_code ec, size_t bytes) override
        {
            handler_.template complete_now(ec, bytes);
        }
    };

    template<class THandler>
    struct write_job final : public write_job_base
    {
        using handler_t = boost::beast::async_base<THandler, executor_type>;
    public:
        template<class U>
        write_job(U&& h, Connection* self) :
                write_job_base(self->get_impl()),
                handler_(std::forward<U>(h), self->get_executor())
        {}
        ~write_job() override = default;

    private:
        handler_t handler_;

        void complete_impl(error_code ec, size_t bytes) override
        {
            handler_.template complete_now(ec, bytes);
        }
    };

    struct run_read
    {
        template<typename THandler>
        void operator()(THandler&& h, Connection *self, lock_type &lck);
    };

    struct run_write
    {
        template<typename THandler>
        void operator()(THandler&& h, Connection *self, lock_type &lck);
    };

    void initiate_read(lock_type& lck, read_job_base* job);
    void initiate_read_impl(lock_type& lck, read_job_base* job);

    void initiate_write(lock_type& lck, write_job_base* job);
    void initiate_write_impl(lock_type& lck, write_job_base* job);
};


template<class THandler>
auto Connection::async_read(THandler&& h) -> BOOST_ASIO_INITFN_RESULT_TYPE(THandler, void(error_code, size_t))
{
    lock_type lck(mutex());
    return boost::asio::async_initiate<THandler, void(error_code, size_t)>(run_read{}, h, this, lck);
}
template<class THandler>
auto Connection::async_write(THandler&& h) -> BOOST_ASIO_INITFN_RESULT_TYPE(THandler, void(error_code, size_t))
{
    lock_type lck(mutex());
    return boost::asio::async_initiate<THandler, void(error_code, size_t)>(run_write{}, h, this, lck);
}

template<typename THandler>
void Connection::run_read::operator()(THandler&& h, Connection *self, lock_type &lck)
{
    BOOST_ASIO_READ_HANDLER_CHECK(THandler, h) type_check;

    using job_t = read_job<typename std::decay_t<THandler>>;
    auto job = new job_t(std::forward<THandler>(h), self);
    self->initiate_read(lck, job);
}

template<typename THandler>
void Connection::run_write::operator()(THandler&& h, Connection *self, lock_type &lck)
{
    BOOST_ASIO_READ_HANDLER_CHECK(THandler, h) type_check;

    using job_t = write_job<typename std::decay_t<THandler>>;
    auto job = new job_t(std::forward<THandler>(h), self);

    self->initiate_write(lck, job);
}

}