//
// Created by focus on 20.12.2021.
//

#pragma once


namespace sdk {


class ISession {
public:
    virtual ~ISession() = default;

    virtual void start() = 0;
    virtual void cancel() = 0;
    virtual void stop() = 0;
};


}

