//
// Created by focus on 13.12.2021.
//

#ifndef ANALYTICS_CONTEXT_H
#define ANALYTICS_CONTEXT_H

#include <boost/asio/io_context.hpp>

#include <memory>


class Context
{
public:
    Context();

    boost::asio::io_context& getContext();

    ~Context();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};


#endif //ANALYTICS_CONTEXT_H
