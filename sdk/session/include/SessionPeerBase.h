//
// Created by focus on 21.12.2021.
//

#pragma once

#include <session/include/SessionBase.h>

#include <boost/asio/io_context.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http/parser.hpp>

#include <session/src/HandleRequest.h>
#include <session/src/SendOp.h>


namespace sdk {


template<class Stream>
class SessionPeerBase :
    public std::enable_shared_from_this<SessionPeerBase<Stream>>,
    public SessionBase
{
public:
    using SessionType = SessionPeerBase<Stream>;
    using Buffer = boost::beast::flat_buffer;
    using Request = boost::beast::http::request<boost::beast::http::string_body>;

    template<typename ...Args>
    explicit SessionPeerBase(boost::asio::io_context& context, Args&& ...args) :
        SessionBase(context),
        stream(std::forward<Args>(args)...)
    {
//        stream.set_option(boost::asio::ip::tcp::no_delay(true));
        lg_.info("create");
    }

    ~SessionPeerBase() override
    {
        lg_.info("destroy");
    }

protected:
    Stream& peer() { return stream; }

    void doRead() {
        lg_.info("start read request");

        namespace bb = boost::beast;

        bb::http::async_read(peer(), buffer_, request_,
                   [this, s=this->shared_from_this()](bb::error_code code, size_t /*bytes*/)
                   {
                       if (code == boost::beast::http::error::end_of_stream)
                       {
                           peer().shutdown(boost::asio::ip::tcp::socket::shutdown_send, code);
                           return;
                       }

                        lg_.info_f("Have request:\n%1%", request_);
                        HandleRequest(request_,
                                      [this, s](boost::beast::http::response<boost::beast::http::empty_body> response)
                                      {
                                        lg_.info("ready response");
                                        auto* op = new SendOp<SessionType>(*this);
                                        if (op)
                                        {
                                            op->template operator()(std::move(response));
                                            lg_.info("send response");
                                        }
                                      });
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
    Buffer buffer_{8128};
    Request request_;

    friend SendOp<SessionType>;
};


}

