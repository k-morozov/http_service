//
// Created by focus on 27.01.2022.
//

#include "Core.h"


namespace sdk {


Core::Core(io_context & io) :
    io_(io),
    lg_("sdk", "Core")
{
    lg_.info("create");
}

Core::~Core()
{
    lg_.info("destroy");
}

void Core::transact_op_base::complete()
{
    complete_impl();
    delete this;
}

void Core::pipeline(Core* self, Connection::request_t request)
{
    for(auto const& action : self->request_pipeline_)
    {
        if (!action)
            break;
        action(request);
    }
}

void Core::add_request_action(request_action action)
{
    // @TODO sync?
    request_pipeline_.push_back(std::move(action));
}

} // namespace sdk