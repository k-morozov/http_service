//
// Created by focus on 20.12.2021.
//

#ifndef ANALYTICS_SESSION_H
#define ANALYTICS_SESSION_H

#include "ISession.h"

#include <common/logger/Logger.h>

#include <boost/asio/io_context.hpp>
#include <boost/asio/io_context_strand.hpp>


namespace sdk {


class SessionBase : public ISession {
public:
    explicit SessionBase(boost::asio::io_context & context);
    ~SessionBase() override;

    void start() override;

    void cancel() override;

    void stop() override;


protected:
    boost::asio::io_context::strand strand_;
    logger_t lg_;

private:
    virtual void doStart() = 0;
    virtual void doCancel() = 0;
    virtual void doStop() = 0;
};


}


#endif //ANALYTICS_SESSION_H
