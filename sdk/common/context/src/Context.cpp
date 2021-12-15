//
// Created by focus on 13.12.2021.
//

#include <common/context/include/Context.h>
#include <common/logger/Logger.h>

#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>

#include <iostream>
#include <thread>
#include <shared_mutex>


class Context::Impl final
{
public:
    Impl();
    ~Impl();

    boost::asio::io_context& getContext() { return context_; }

private:
    boost::asio::thread_pool pool_;
    boost::asio::io_context context_;

    mutable std::shared_mutex mut_;
    bool destroy_ = false;

    logger_t lg_;

private:
    void addThread();
    void dispose() noexcept;
    bool disposed() const;
    void setDisposed();

    void run();
};

Context::Impl::Impl() :
    pool_(2),
    lg_("sdk", "Context")
{
    addThread();
    addThread();

    lg_.info("create");
}

Context::Impl::~Impl()
{
    setDisposed();

    dispose();
    lg_.info("destroy");
}

void Context::Impl::addThread() {
    if (disposed())
        return;

    boost::asio::post(pool_, [this]{ run(); });
}

void Context::Impl::dispose() noexcept
{
    try {
        // @TODO check destroy?

        context_.stop();
        lg_.info("context stopped");

        pool_.stop();
        lg_.info("pool stopped");

        [[maybe_unused]]
        auto _ = std::async(std::launch::async,
                            [&pool=pool_] { pool.join(); });
    } catch (std::exception const& ex)
    {
        lg_.error_f("exception when destroy: %1%", ex.what());
    }
}

bool Context::Impl::disposed() const
{
    std::shared_lock lock(mut_);
    return destroy_;
}

void Context::Impl::setDisposed()
{
    std::unique_lock lock(mut_);
    destroy_ = true;
}

void Context::Impl::run()
{
    while (true)
    {
        try {
            lg_.info_f("Impl::run %1%", std::this_thread::get_id());
            auto guard = boost::asio::make_work_guard(context_);
            context_.run();
            break;
        } catch (std::exception const& ex)
        {
            lg_.error_f("exception in context thread: %1%", ex.what());
        }
    }
    lg_.info_f("finish context thread, id=%1%", std::this_thread::get_id());
}




Context::Context()
{
    impl_ = std::make_unique<Context::Impl>();
    if (!impl_)
        throw std::bad_alloc();
}

Context::~Context()
{
}

boost::asio::io_context &Context::getContext() {
    return impl_->getContext();
}
