//
// Created by focus on 06.12.2021.
//

#ifndef ANALYTICS_LOGGERIMPL_H
#define ANALYTICS_LOGGERIMPL_H

#include <boost/log/sources/severity_channel_logger.hpp>
#include "Severity.h"
#include "Utils.h"

class LoggerImpl : public boost::log::sources::severity_channel_logger< boost::log::trivial::severity_level >
{
public:
    using Base = boost::log::sources::severity_channel_logger< boost::log::trivial::severity_level >;

    LoggerImpl(std::string const& channel_name);

    void write(sdk::Severity level, std::string const& text);

    void init();

private:
    // write
};


#endif //ANALYTICS_LOGGERIMPL_H
