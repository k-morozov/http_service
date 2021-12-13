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

class Context::Impl final
{
public:
    Impl();
    ~Impl();

    boost::asio::io_context& getContext() { return context_; }

private:
    boost::asio::thread_pool pool_;
    boost::asio::io_context context_;

    logger_t lg_;

private:
    void addThread();

    void run();
};

Context::Impl::Impl() :
    pool_(2),
    lg_("sdk", "Context")
{
    addThread();
    addThread();
}

Context::Impl::~Impl()
{
    pool_.stop();
}

void Context::Impl::addThread() {
    boost::asio::post(pool_, [this]{ run(); });
}

void Context::Impl::run()
{
    while (true)
    {
        try {
            std::stringstream ss;
            ss << "Impl::run " << std::this_thread::get_id();
            lg_.info(ss.str());
            auto guard = boost::asio::make_work_guard(context_);
            context_.run();
            break;
        } catch (std::exception const& ex)
        {
            // add log?
        }
    }
    lg_.info("finish impl::run");
}




Context::Context()
{
    impl_ = std::make_unique<Context::Impl>();
}

Context::~Context()
{

}

boost::asio::io_context &Context::getContext() {
    return impl_->getContext();
}
