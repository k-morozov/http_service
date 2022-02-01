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

#include <shared_mutex>


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
    using request_action = std::function<error_code (Connection::request_t const&)>;
    using request_pipeline_t = std::vector<request_action>;
    using mutex_type = std::shared_mutex;
    using lock_read_type = std::shared_lock<mutex_type>;
    using lock_write_type = std::unique_lock<mutex_type>;


    explicit Core(io_context & io);
    ~Core();

    io_context& get_context() { return io_; }

    template<typename THandler>
    BOOST_ASIO_INITFN_RESULT_TYPE(THandler, void(error_code)) async_transact(socket_t socket, THandler&& h);

    static void pipeline(Core* self, Connection::request_t const& request);

    void add_request_action(request_action);

private:
    io_context& io_;
    logger_t lg_;
    mutex_type m_;

    request_pipeline_t request_pipeline_;


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
    struct transact_op final:
            public transact_op_base,
            private boost::asio::coroutine
    {
        using handler_t = boost::beast::async_base<T, executor_type>;

        Core* self_;
        handler_t h_;
        logger_t lg_;

        template<typename U>
        transact_op(U&& h, Core* self, socket_t socket) :
            transact_op_base(std::move(socket)),
            self_(self),
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
                                lg_.template info_f("read completed: %1%, bytes=%2%",
                                        ec.message(), bytes);
                                (*this)();
                            });
                Core::pipeline(self_, con_->request());
                con_->set_response({});

                yield con_->template async_write([this](error_code ec, size_t bytes)
                                                 {
                                                     lg_.template info_f("write completed: %1%, bytes=%2%",
                                                                         ec.message(), bytes);
                                                     (*this)();
                                                 });
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
auto Core::async_transact(socket_t socket, THandler&& h) -> BOOST_ASIO_INITFN_RESULT_TYPE(THandler, void(error_code))
{
    return boost::asio::async_initiate<THandler, void(error_code)>(run_transact{}, h, this, std::move(socket));
};

} // namespace sdk

