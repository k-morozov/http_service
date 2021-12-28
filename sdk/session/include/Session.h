//
// Created by focus on 27.12.2021.
//

#pragma once


#include <session/include/SessionPeerBase.h>


namespace sdk
{

using Stream = boost::asio::ip::tcp::socket;

class Session final :
        public SessionPeerBase<Stream>
{
    using base = SessionPeerBase<Stream>;

public:
    explicit Session(boost::asio::io_context& io, Stream stream) :
            base(io, std::move(stream))
    {}

    void start() override {
        SessionBase::start();
    }

    void cancel() override {
        SessionBase::cancel();
    }

    void stop() override {
        SessionBase::stop();
    }


private:

};


}
