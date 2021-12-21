//
// Created by focus on 21.12.2021.
//

#pragma once

#include <session/include/SessionBase.h>

#include <boost/asio/io_context.hpp>
#include <boost/beast/core/flat_buffer.hpp>

namespace sdk {


template<class Stream>
class Session :
    public std::enable_shared_from_this<Session<Stream>>,
    public SessionBase
{
public:
    using Buffer = boost::beast::flat_buffer;
    using Request = boost::beast::http::request<boost::beast::http::string_body>;

    template<typename ...Args>
    explicit Session(boost::asio::io_context& context, Args&& ...args) :
        SessionBase(context),
        stream(std::forward<Args>(args)...)
    {}
    ~Session() = default;

protected:
    Stream& peer() { return stream; }

    void doRead() {
        lg_.info("start read");

        using boost::beast::http::async_read;

        async_read(peer(), buffer_, request_,
                   [s=this->shared_from_this(), this](boost::beast::error_code code, size_t bytes)
                   {
                        lg_.info("Have request");
                        doRead();
                   });
    }

protected:
    void doStart() override {
        doRead();
    }

    void doCancel() override {

    }

    void doStop() override {

    }

private:
    Stream stream;
    Buffer buffer_{8192};
    Request request_;
};


}

