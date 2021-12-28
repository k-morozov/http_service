//
// Created by focus on 22.12.2021.
//

#pragma once


#include <boost/beast/http.hpp>
#include <boost/beast/http/fields.hpp>
#include <boost/beast/version.hpp>


template<class TBody, class TAllocator, class TSend>
void HandleRequest(boost::beast::http::request<TBody, boost::beast::http::basic_fields<TAllocator>> request, TSend sender)
{
    boost::beast::http::response<boost::beast::http::empty_body> response {boost::beast::http::status::ok, request.version()};
    response.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(boost::beast::http::field::content_type, "text/html");
    response.content_length(0);
    response.keep_alive(true);
    return sender(std::move(response));
};