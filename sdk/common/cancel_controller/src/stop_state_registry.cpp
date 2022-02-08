//
// Created by focus on 08.02.2022.
//

#include "stop_state_registry.h"

namespace sdk {


void stop_state_registry::push(stop_state_ptr p)
{
    if (!p)
        return;

    lock_t lck(mutex_);
    lg_.info_f("push, ptr=%1%", p.get());
    pending_.push_back(p);
    notify(lck, std::move(p));
}

void stop_state_registry::notify(lock_t& lck, stop_state_ptr p)
{

}


} // namespace sdk