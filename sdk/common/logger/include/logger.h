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
    explicit Logger(sdk::Severity level);
    ~Logger();

//    Logger(std::string const& tag);

    void info(std::string const& message);

private:
    std::unique_ptr<LoggerImpl> lg_;
};
#endif //ANALYTICS_LOGGER_H
