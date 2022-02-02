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

    // @TODO add sync
    self->request_pipeline_->run(request);
}

void Core::setup_pipeline(PipelinePtr pipeline)
{
    // @TODO add sync
    request_pipeline_ = std::move(pipeline);
}

} // namespace sdk