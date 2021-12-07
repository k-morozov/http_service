//
// Created by focus on 06.12.2021.
//

#ifndef ANALYTICS_LOGGERIMPL_H
#define ANALYTICS_LOGGERIMPL_H

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sinks/async_frontend.hpp>

#include "Severity.h"
#include "Utils.h"

class LoggerImpl : public boost::log::sources::severity_channel_logger< boost::log::trivial::severity_level >
{
public:
    using Base = boost::log::sources::severity_channel_logger< boost::log::trivial::severity_level >;

    template<typename T>
    using shared_ptr = boost::shared_ptr<T>;

    template<typename T>
    using asynchronous_sink = boost::log::sinks::asynchronous_sink<T>;

    using text_ostream_backend = boost::log::sinks::text_ostream_backend;


    explicit LoggerImpl(std::string const& channel_name, std::string const& tag_name);
    ~LoggerImpl();

    void write(sdk::Severity level, std::string const& text);

    void init();

private:
    using text_sink =  asynchronous_sink< boost::log::sinks::text_ostream_backend >;
    shared_ptr<text_sink> sink_;

    static void stop_logging(shared_ptr<asynchronous_sink<text_ostream_backend>>& sink);
};


#endif //ANALYTICS_LOGGERIMPL_H
