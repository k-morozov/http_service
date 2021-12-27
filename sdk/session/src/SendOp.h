//
// Created by focus on 27.12.2021.
//

#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>


namespace sdk
{


template<class T>
struct SendLambda
{
    using SessionType = T;

    SessionType& self_;

    explicit SendLambda(T & self) : self_(self) {}

    template<class Body, class Fields>
    void operator()(boost::beast::http::message<false, Body, Fields> message) const
    {
        auto sp = std::make_shared<
        boost::beast::http::message<false, Body, Fields> >(std::move(message));
        self_.response_ = sp;

        auto self = self_.shared_from_this();
        boost::beast::http::async_write(self_.peer(),
                                        *sp,
                                        [&p = self_, self](auto&& a, auto&& b)
                                        {
                                            p.lg_.info("write done");
                                            p.doRead();
                                        });
    }
};


}
