//
// Created by focus on 04.12.2021.
//

#ifndef ANALYTICS_LOGGER_H
#define ANALYTICS_LOGGER_H

#include "Severity.h"
#include "LoggerImpl.h"

#include <memory>

template<bool mt>
class Logger_t : public LoggerImpl<mt>
{
    using base_type = LoggerImpl<mt>;
public:
    explicit Logger_t(std::string const& channel_name, std::string const& tag)
        : base_type(channel_name, tag)
    {}

    ~Logger_t() override = default;
};

using sdk_logger_t = Logger_t<false>;
using sdk_logger_mt = Logger_t<true>;

#endif //ANALYTICS_LOGGER_H
