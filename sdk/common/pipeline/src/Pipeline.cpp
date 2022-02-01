//
// Created by focus on 01.02.2022.
//

#include "Pipeline.h"

namespace sdk {


Pipeline::Pipeline() : lg_("sdk", "pipeline")
{
    lg_.info("create");
}


void Pipeline::run(request_t message)
{
    [[maybe_unused]]
    lock_read_type lck(m_);

    for(auto const& action : request_pipeline_)
    {
        try {
            if (!action)
            {
                lg_.warning("broken action");
                continue;
            }
            auto ec = action(std::move(message));
            if (ec)
            {
                lg_.error_f("failed by error: ec=%1%, message=%2%", ec, ec.message());
                break;
            }
        } catch (std::exception const& ex)
        {
            lg_.error_f("failed by except: %1%", ex.what());
        }

    }
}

void Pipeline::append_handler(request_handler_t action)
{
    [[maybe_unused]]
    lock_write_type lck(m_);

    request_pipeline_.push_back(std::move(action));
}


} // namespace sdk