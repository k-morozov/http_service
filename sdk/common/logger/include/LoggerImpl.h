//
// Created by focus on 06.12.2021.
//

#ifndef ANALYTICS_LOGGERIMPL_H
#define ANALYTICS_LOGGERIMPL_H

#include <fstream>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/utility/setup/file.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/attributes.hpp>


#include "Severity.h"
#include "common/logger/src/Utils.h"

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

namespace detail {
    BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
    BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", boost::log::trivial::severity_level)
    BOOST_LOG_ATTRIBUTE_KEYWORD(timeline, "Timeline", boost::log::attributes::timer::value_type)
    BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)
    BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
}

template<bool mt = false>
class LoggerImpl
{
public:
    using BaseType = typename std::conditional_t<
            mt,
            boost::log::sources::severity_channel_logger_mt< boost::log::trivial::severity_level>,
            boost::log::sources::severity_channel_logger< boost::log::trivial::severity_level>>;

    template<typename T>
    using shared_ptr = boost::shared_ptr<T>;

    template<typename T>
    using asynchronous_sink = boost::log::sinks::asynchronous_sink<T>;

    using text_ostream_backend = boost::log::sinks::text_ostream_backend;


    explicit LoggerImpl(std::string const& channel_name, std::string const& tag_name);
    virtual ~LoggerImpl();

    void trace(std::string const& message);

    void debug(std::string const& message);

    void info(std::string const& message);

    void warning(std::string const& message);

    void error(std::string const& message);

    void fatal(std::string const& message);

private:
    BaseType impl_;

    using text_sink =  asynchronous_sink< boost::log::sinks::text_ostream_backend >;
    shared_ptr<text_sink> sink_;

    static void stop_logging(shared_ptr<asynchronous_sink<text_ostream_backend>>& sink);

    void init();

    void write(sdk::Severity level, std::string const& text);
};


template<bool mt>
LoggerImpl<mt>::LoggerImpl(std::string const& channel_name, std::string const& tag_name) :
        impl_(keywords::severity=boost::log::trivial::severity_level::info,
            keywords::channel=channel_name)
{
    impl_.add_attribute("Tag", attrs::constant<std::string>(tag_name));
    init();
}

template<bool mt>
LoggerImpl<mt>::~LoggerImpl()
{
    stop_logging(sink_);
}

template<bool mt>
void LoggerImpl<mt>::write(sdk::Severity level, std::string const& text)
{
    BOOST_LOG_SEV(impl_, Utils::convert(level)) << text;
}

template<bool mt>
void LoggerImpl<mt>::trace(const std::string &message) {
    write(sdk::Severity::Trace, message);
}

template<bool mt>
void LoggerImpl<mt>::debug(const std::string &message) {
    write(sdk::Severity::Debug, message);
}

template<bool mt>
void LoggerImpl<mt>::info(std::string const& message)
{
    write(sdk::Severity::Info, message);
}

template<bool mt>
void LoggerImpl<mt>::warning(const std::string &message) {
    write(sdk::Severity::Warning, message);
}

template<bool mt>
void LoggerImpl<mt>::error(const std::string &message) {
    write(sdk::Severity::Error, message);
}

template<bool mt>
void LoggerImpl<mt>::fatal(const std::string &message) {
    write(sdk::Severity::Fatal, message);
}

template<bool mt>
void LoggerImpl<mt>::init()
{
    logging::add_file_log(
            keywords::rotation_size = 10 * 1024 * 1024,
            keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0)
    );

    logging::formatter fmt = expr::stream
            << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S ")
            << "<" << detail::severity << "> "
            << expr::if_(expr::has_attr(detail::channel))
            [
                    expr::stream << "(" << detail::channel << ") "
            ]
            << expr::if_(expr::has_attr(detail::tag_attr))
            [
                    expr::stream << "[" << detail::tag_attr << "] "
            ]
            << expr::smessage;

    auto sink = boost::make_shared<text_sink>();
    sink->locked_backend()->add_stream(
            shared_ptr<std::ostream>(&std::cout, boost::null_deleter{})
    );
    sink->locked_backend()->add_stream(
            boost::make_shared<std::ofstream>("sample.log")
    );
    sink->set_formatter(fmt);
    logging::core::get()->add_sink(sink);


    logging::add_common_attributes();

    logging::core::get()->set_filter(
            logging::trivial::severity >= logging::trivial::info
    );

    sink_.swap(sink);

    stop_logging(sink);
}

template<bool mt>
void LoggerImpl<mt>::stop_logging(boost::shared_ptr<asynchronous_sink<text_ostream_backend > >& sink)
{
    if (!sink)
        return;

    boost::shared_ptr< logging::core > core = logging::core::get();

    // Remove the sink from the core, so that no records are passed to it
    core->remove_sink(sink);

    // Break the feeding loop
    sink->stop();

    // Flush all log records that may have left buffered
    sink->flush();

    sink.reset();
}

#endif //ANALYTICS_LOGGERIMPL_H
