//
// Created by focus on 04.12.2021.
//

#ifndef ANALYTICS_LOGGER_H
#define ANALYTICS_LOGGER_H

#include "Severity.h"

#include <memory>

class LoggerImpl;

class Logger
{
public:
    explicit Logger(std::string const& channel_name, std::string const& tag);
    ~Logger();

    void trace(std::string const& message);

    void debug(std::string const& message);

    void info(std::string const& message);

    void warning(std::string const& message);

    void error(std::string const& message);

    void fatal(std::string const& message);

private:
    void write(sdk::Severity level, std::string const& message);

    std::unique_ptr<LoggerImpl> lg_;
};


#endif //ANALYTICS_LOGGER_H
