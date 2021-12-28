//
// Created by focus on 27.12.2021.
//

#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>


namespace sdk
{


template<class T>
class SendOp
{
    using SessionType = T;
    using TResPtr = std::shared_ptr<void>;

    SessionType& session_;
    TResPtr response_;

public:

    explicit SendOp(T & self) : session_(self) {}

    template<class TBody, class TFields>
    void operator()(boost::beast::http::message<false, TBody, TFields> message)
    {
        using TMessage = boost::beast::http::message<false, TBody, TFields>;

        auto sp = std::make_shared<TMessage>(std::move(message));
        response_ = sp;

        auto session = session_.shared_from_this();
        boost::beast::http::async_write(session_.peer(),
                                        message,
                                        [session, this](auto&& a, auto&& b)
                                        {
                                            session->lg_.info("write done");
                                            session->doRead();
                                            delete this;
                                        });
    }
};


}
