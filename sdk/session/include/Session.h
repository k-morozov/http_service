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
class Session :
    public std::enable_shared_from_this<Session<Stream>>,
    public SessionBase
{
public:
    using Buffer = boost::beast::flat_buffer;
    using Request = boost::beast::http::request<boost::beast::http::string_body>;
    using Parser = boost::beast::http::request_parser<boost::beast::http::string_body>;
    using Response = std::shared_ptr<void>;

    template<typename ...Args>
    explicit Session(boost::asio::io_context& context, Args&& ...args) :
        SessionBase(context),
        stream(std::forward<Args>(args)...)
    {
//        stream.set_option(boost::asio::ip::tcp::no_delay(true));
        lg_.info("create");
    }

    ~Session() override
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
                                            lambda_(std::move(response));
                                            lg_.info("send response");

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
    Parser parser_{};


    friend SendLambda<Session<Stream>>;
    SendLambda<Session<Stream>> lambda_ {*this};

    Request request_;
    Response response_;
};



}

