//
// Created by focus on 04.12.2021.
//
#include "pch.h"

#include "logger.h"

#include "LoggerImpl.h"


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

void Logger::write(sdk::Severity level, std::string const& message)
{
    lg_->write(level, message);
}

void Logger::info(std::string const& message)
{
    write(sdk::Severity::Info, message);
}