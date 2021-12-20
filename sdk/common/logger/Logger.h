//
// Created by focus on 04.12.2021.
//

#ifndef ANALYTICS_LOGGER_H
#define ANALYTICS_LOGGER_H

#include "Severity.h"
#include "LoggerBase.h"

#include <memory>

namespace Log {

    template<bool mt>
    class Logger final : public LoggerBase<mt> {
        using base_type = LoggerBase<mt>;

    public:
        template<typename Arg, typename ...Args>
        explicit Logger(Arg && arg, Args &&... args)
                : base_type(std::forward<Arg>(arg), std::forward<Args>(args)...) {}

        ~Logger() override = default;
    };

}

using logger_t = Log::Logger<false>;
using logger_mt = Log::Logger<true>;


#endif //ANALYTICS_LOGGER_H
