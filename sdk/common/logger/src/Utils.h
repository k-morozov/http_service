//
// Created by focus on 06.12.2021.
//

#ifndef ANALYTICS_UTILS_H
#define ANALYTICS_UTILS_H

#include "Severity.h"

#include <boost/log/trivial.hpp>

class Utils
{
public:
    using severity_level = boost::log::trivial::severity_level;


    static severity_level convert(sdk::Severity const level)
    {
        switch (level) {
            case sdk::Severity::Info:
                return severity_level::info;
            case sdk::Severity::Debug :
                return severity_level::debug;
            case sdk::Severity::Error :
                return severity_level::error;
            case sdk::Severity::Fatal :
                return severity_level::fatal;
            case sdk::Severity::Warning :
                return severity_level::warning;

            default:
                return severity_level::trace;
        }
    }
};


#endif //ANALYTICS_UTILS_H
