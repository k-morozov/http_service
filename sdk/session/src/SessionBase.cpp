//
// Created by focus on 20.12.2021.
//

#include <session/include/SessionBase.h>


namespace sdk {


SessionBase::SessionBase(boost::asio::io_context & context) :
    strand_(context),
    lg_("sdk", "Session")
{
    lg_.info("create");
}

SessionBase::~SessionBase()
{
    lg_.info("destroy");
}

void SessionBase::start() {
    // timer
    doStart();
}

void SessionBase::cancel() {
    // timer
}

void SessionBase::stop() {
    // timer
}


}