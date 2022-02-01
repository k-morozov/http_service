//
// Created by focus on 01.02.2022.
//

#pragma once

#include <common/logger/Logger.h>

#include <boost/beast.hpp>

#include <shared_mutex>


namespace sdk {

class Pipeline;
using PipelinePtr = std::unique_ptr<Pipeline>;


class Pipeline {
public:
    using body_t = boost::beast::http::string_body;
    using request_t = boost::beast::http::request<body_t>;
    using error_code = boost::system::error_code;
    using request_handler_t = std::function<error_code (request_t)>;
    using mutex_type = std::shared_mutex;
    using lock_read_type = std::shared_lock<mutex_type>;
    using lock_write_type = std::unique_lock<mutex_type>;


    Pipeline();
    ~Pipeline() = default;

    void run(request_t message);

    void append_handler(request_handler_t h);

private:
    logger_t lg_;
    std::vector<request_handler_t> request_pipeline_;
    mutex_type m_;
};


} // namespace sdk



