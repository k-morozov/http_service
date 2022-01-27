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


} // namespace sdk