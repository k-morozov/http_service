//
// Created by focus on 04.12.2021.
//
#include "pch.h"

#include "logger.h"

#include "LoggerImpl.h"


//auto init()
//{
//    logging::add_file_log(
//            keywords::rotation_size = 10 * 1024 * 1024,
//            keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0)
//    );
//
//    logging::formatter fmt = expr::stream
//            << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S ")
//            << "<" << severity << "> "
//            << expr::if_(expr::has_attr(channel))
//            [
//                    expr::stream << "(" << channel << ") "
//            ]
//            << expr::if_(expr::has_attr(tag_attr))
//            [
//                    expr::stream << "[" << tag_attr << "] "
//            ]
//            << expr::smessage;
//
//    //! create thread for async run
//    using text_sink =  sinks::asynchronous_sink< sinks::text_ostream_backend >;
//    auto sink = boost::make_shared<text_sink>();
//    sink->locked_backend()->add_stream(
//            boost::shared_ptr<std::ostream>(&std::cout, boost::null_deleter{})
//    );
//    sink->locked_backend()->add_stream(
//            boost::make_shared<std::ofstream>("sample.log")
//    );
//    sink->set_formatter(fmt);
//    logging::core::get()->add_sink(sink);
//
//
//    logging::add_common_attributes();
//
//    logging::core::get()->set_filter(
//            logging::trivial::severity >= logging::trivial::info
//            );
//
//    return sink;
//}
//
//void logging_function()
//{
//    src::severity_channel_logger< boost::log::trivial::severity_level > slg(
//            keywords::severity=boost::log::trivial::severity_level::info,
//            keywords::channel="channel's name");
//
//    slg.add_attribute("Tag", attrs::constant<std::string>("tag_test"));
//
//    BOOST_LOG(slg) << "Normal level";
//    BOOST_LOG_SEV(slg, boost::log::trivial::severity_level::info) << "A regular message";
//    BOOST_LOG_SEV(slg, boost::log::trivial::severity_level::warning) << "Something bad is going on but I can handle it";
//    BOOST_LOG_SEV(slg, boost::log::trivial::severity_level::fatal) << "Everything crumbles, shoot me now!";
//
//    BOOST_LOG_SEV(slg, boost::log::trivial::severity_level::trace) << "An important message";
//}
//
//void stop_logging(boost::shared_ptr< sinks::asynchronous_sink< sinks::text_ostream_backend > >& sink)
//{
//    boost::shared_ptr< logging::core > core = logging::core::get();
//
//    // Remove the sink from the core, so that no records are passed to it
//    core->remove_sink(sink);
//
//    // Break the feeding loop
//    sink->stop();
//
//    // Flush all log records that may have left buffered
//    sink->flush();
//
//    sink.reset();
//}

//void Logger::hello()
//{
//    auto sink = init();
//    logging_function();
//
//    stop_logging(sink);
//}


Logger::Logger(sdk::Severity)
{
    lg_ = std::make_unique<LoggerImpl>("Channel's name");
    if (!lg_)
        throw std::bad_alloc();
    lg_->init();
}

Logger::~Logger() = default;


void Logger::info(std::string const& message)
{
    lg_->info(message);
}