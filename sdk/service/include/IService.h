//
// Created by focus on 12.12.2021.
//

#ifndef ANALYTICS_ISERVICE_H
#define ANALYTICS_ISERVICE_H


class IService {
public:
    virtual void run() = 0;
    virtual void stop() = 0;

    virtual ~IService() = default;
};


#endif //ANALYTICS_ISERVICE_H
