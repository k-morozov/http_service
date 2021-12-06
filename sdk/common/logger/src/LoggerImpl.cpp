//
// Created by focus on 06.12.2021.
//

#include "pch.h"

#include "LoggerImpl.h"

#include <iostream>
#include <iomanip>
#include <fstream>

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
#include <boost/log/utility/setup/file.hpp>


namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

namespace detail {
    BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
    BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", boost::log::trivial::severity_level)
    BOOST_LOG_ATTRIBUTE_KEYWORD(timeline, "Timeline", attrs::timer::value_type)
    BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)
    BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
}



LoggerImpl::LoggerImpl(std::string const& channel_name) :
    Base(keywords::severity=boost::log::trivial::severity_level::info,
        keywords::channel=channel_name)
{
    add_attribute("Tag", attrs::constant<std::string>("tag_test"));
}

void LoggerImpl::info(std::string const& text)
{
    BOOST_LOG(*this) << text;
}

void LoggerImpl::init()
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

    //! create thread for async run
    using text_sink =  sinks::asynchronous_sink< sinks::text_ostream_backend >;
    auto sink = boost::make_shared<text_sink>();
    sink->locked_backend()->add_stream(
            boost::shared_ptr<std::ostream>(&std::cout, boost::null_deleter{})
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

//    return sink;
}