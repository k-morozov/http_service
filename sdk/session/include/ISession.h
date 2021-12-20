//
// Created by focus on 20.12.2021.
//

#ifndef ANALYTICS_ISESSION_H
#define ANALYTICS_ISESSION_H


namespace sdk {


class ISession {
public:
    virtual ~ISession() = default;

    virtual void start() = 0;
    virtual void cancel() = 0;
    virtual void stop() = 0;
};


}


#endif //ANALYTICS_ISESSION_H
