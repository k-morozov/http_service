//
// Created by focus on 01.02.2022.
//

#include "Pipeline.h"

namespace sdk {


Pipeline::Pipeline() :
    controller_(Controller::create()),
    lg_("sdk", "pipeline")
{
    controller_->start_work();
    lg_.info("create");
}

Pipeline::~Pipeline()
{
    controller_->end_work();
}


void Pipeline::run(request_t const message)
{
    [[maybe_unused]]
    shared_lock_t lck(m_);

    for(auto const& action : request_pipeline_)
    {
        try {
            if (controller_->is_cancel())
            {
                lg_.warning("cancel from controller");
                break;
            }

            if (!action)
            {
                lg_.warning("broken action");
                continue;
            }
            auto ec = action(message);
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
    unique_lock_t lck(m_);

    request_pipeline_.push_back(std::move(action));
}

ControllerPtr Pipeline::get_controller()
{
    return controller_;
}


} // namespace sdk