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

void Core::pipeline(Core* self, Connection::request_t const& request)
{
    if (!self)
        return;

    lock_read_type lck(self->m_);

    for(auto const& action : self->request_pipeline_)
    {
        try {
            if (!action)
            {
                self->lg_.warning("broken action in pipeline");
                continue;
            }
            auto ec = action(request);
            if (ec)
            {
                self->lg_.error_f("pipeline failed by error: ec=%1%, message=%2%", ec, ec.message());
                break;
            }
        } catch (std::exception const& ex)
        {
            self->lg_.error_f("pipeline failed by except: %1%", ex.what());
        }

    }
}

void Core::add_request_action(request_action action)
{
    lock_write_type lck(m_);
    request_pipeline_.push_back(std::move(action));
}

} // namespace sdk