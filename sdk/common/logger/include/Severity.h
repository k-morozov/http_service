//
// Created by focus on 06.12.2021.
//

#ifndef ANALYTICS_SEVERVITY_H
#define ANALYTICS_SEVERVITY_H

namespace sdk {

    enum class Severity : unsigned
    {
        Trace = 1,
        Debug,
        Info,
        Warning,
        Error,
        Fatal
    };

}

#endif //ANALYTICS_SEVERVITY_H
