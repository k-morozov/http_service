//
// Created by focus on 02.02.2022.
//

#include <common/cancel_controller/include/Controller.h>


namespace sdk {


Controller::~Controller()
{
    lock_t lck(m_);
    work_ = false;
    cv_.notify_all();
}

void Controller::subscribe(finished_t handler)
{
    lock_t lck(m_);
    handler_.connect(handler);
}

bool Controller::is_cancel() const
{
    lock_t lck(m_);
    return cancel_;
}

void Controller::start_work()
{
    lock_t lck(m_);
    work_ = true;
}

void Controller::end_work()
{
    lock_t lck(m_);
    work_ = false;
    emit_signal();
    cv_.notify_all();
}

void Controller::emit_signal()
{
    handler_();
}

void Controller::cancel()
{
    lock_t lck(m_);
    emit_signal();
    cancel_ = true;
}

void Controller::wait()
{
    lock_t lck(m_);

    while (work_ || count_process_.load()>0)
    {
        cv_.wait(lck, [this]() -> bool
        {
            return !work_ || 0 == count_process_.load();
        });
//        if (!work_ && 0 == count_process_)
//            break;
    }

}

ControllerPtr Controller::create()
{
    return std::make_shared<Controller>();
}

} // namespace sdk