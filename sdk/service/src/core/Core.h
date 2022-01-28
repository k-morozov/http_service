//
// Created by focus on 27.01.2022.
//

#pragma once

#include <connection/include/Connection.h>
#include <common/logger/Logger.h>

#include <boost/asio.hpp>
#include <boost/beast/core/async_base.hpp>
#include <boost/asio/yield.hpp>

#if BOOST_VERSION >= 107400
    #include <boost/asio/executor.hpp>
#else
    #include <boost/asio/any_io_executor.hpp>
#endif


namespace sdk {


class Core
{
public:
    using io_context = boost::asio::io_context;
    using protocol_t = boost::asio::ip::tcp;
    using socket_t = protocol_t::socket;
    using error_code = boost::system::error_code;
#if BOOST_VERSION >= 107400
    using executor_type = boost::asio::any_io_executor;
#else
    using executor_type = boost::asio::executor;
#endif


    explicit Core(io_context & io);
    ~Core();

    io_context& get_context() { return io_; }

    template<typename THandler>
    BOOST_ASIO_INITFN_RESULT_TYPE(THandler, void(error_code)) async_transact(socket_t socket, THandler &&h);

private:
    io_context& io_;
    logger_t lg_;


    struct transact_op_base
    {
        explicit transact_op_base(socket_t socket) :
                con_(std::make_unique<Connection>(std::move(socket)))
        {
            if (!con_)
                throw std::invalid_argument("Failed create connection for transact op.");
        }
        virtual ~transact_op_base() = default;

        void complete();

    protected:
        std::unique_ptr<Connection> con_;

        virtual void complete_impl() = 0;
    };

    template<class T>
    struct transact_op :
            public transact_op_base,
            private boost::asio::coroutine
    {
        using handler_t = boost::beast::async_base<T, executor_type>;

        handler_t h_;
        logger_t lg_;

        template<typename U>
        transact_op(U&& h, socket_t socket) :
            transact_op_base(std::move(socket)),
            h_(std::forward<U>(h), con_->get_executor()),
            lg_("http", "transact")
        {
            lg_.info("create");
            (*this)();
        }
        ~transact_op() override { lg_.info("destroy"); };

        void operator()()
        {
            reenter(this)
            {
                yield con_->template async_read(
                            [this](error_code ec, size_t bytes)
                            {
                                std::cout << "read completed: " << ec.message()
                                          << ", bytes=" << bytes << std::endl;
                                (*this)();
                            });
                std::cout << con_->request() << std::endl;
            }

            if (is_complete())
            {
                lg_.info("coro complete.");
                complete();
            }
        }

        void complete_impl() override
        {
            h_.template complete_now(error_code{});
        }
    };

    struct run_transact
    {
        template<class T, class ...Args>
        void operator()(T&& h, Args&& ...args)
        {
            using type = typename std::decay_t<T>;

            [[maybe_unused]]
            auto _ = new transact_op<type>(std::forward<T>(h), std::forward<Args>(args)...);
        }
    };
};


template<typename THandler>
auto Core::async_transact(socket_t socket, THandler &&h) ->BOOST_ASIO_INITFN_RESULT_TYPE(THandler, void(error_code))
{
    return boost::asio::async_initiate<THandler, void(error_code)>(run_transact{}, h, std::move(socket));
};

} // namespace sdk

