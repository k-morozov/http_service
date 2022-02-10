//
// Created by focus on 02.02.2022.
//

#include <common/cancel_controller/include/Controller.h>


namespace sdk {


Controller::~Controller() = default;

void Controller::subscribe(finished_t handler)
{
    lock_t lck(m_);
    // @TODO thread safe?
    handler_.connect(handler);
}

bool Controller::is_cancel() const
{
    return cancel_.load();
}

void Controller::emit_signal()
{
    handler_();
}

void Controller::cancel()
{
    emit_signal();
    cancel_.store(true);
}

void Controller::wait()
{
    lock_t lck(m_);

    while (count_process_.load()>0)
    {
        cv_.wait(lck, [this]() -> bool
        {
            return 0 == count_process_.load();
        });
    }

}

ControllerPtr Controller::create()
{
    return std::make_shared<Controller>();
}

} // namespace sdk