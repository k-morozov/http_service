//
// Created by focus on 02.02.2022.
//

#pragma once

#include <boost/signals2/signal.hpp>

#include <memory>
#include <mutex>
#include <condition_variable>


namespace sdk {


class Controller;
using ControllerPtr = std::shared_ptr<Controller>;


class Controller
{
    using finish_signal_t = boost::signals2::signal<void()>;
    using finished_t = finish_signal_t::slot_type;
    using mutex_t = std::mutex;
    using lock_t = std::unique_lock<mutex_t>;

public:
    Controller() = default;
    ~Controller();

    static ControllerPtr create();

    void subscribe(finished_t handler);

    bool is_cancel() const;

    void start_work();

    void end_work();

    void cancel();

    void wait();

    template<class F, class ...Args>
    void process(F&& f, Args&& ... args);

private:
    finish_signal_t handler_;

    mutable mutex_t m_;
    std::condition_variable cv_;

    // @TODO change to atomic?
    bool cancel_ = false;
    bool pause_ = false;
    bool resume_ = false;
    bool work_ = false;

    std::atomic_uint  count_process_ = 0;

private:
    void emit_signal();

};


template<class F, class ...Args>
void Controller::process(F&& f, Args&& ... args)
{
    count_process_++;
    f(std::forward<Args>(args)...);
    count_process_--;
}

} // namespace sdk