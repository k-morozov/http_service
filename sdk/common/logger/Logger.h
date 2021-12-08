//
// Created by focus on 04.12.2021.
//

#ifndef ANALYTICS_LOGGER_H
#define ANALYTICS_LOGGER_H

#include "Severity.h"
#include "LoggerImpl.h"

#include <memory>

template<bool mt>
class Logger final : public LoggerImpl<mt>
{
    using base_type = LoggerImpl<mt>;
public:
    explicit Logger(std::string const& channel_name, std::string const& tag)
        : base_type(channel_name, tag)
    {}

    ~Logger() override = default;
};

using logger_t = Logger<false>;
using logger_mt = Logger<true>;

#endif //ANALYTICS_LOGGER_H
