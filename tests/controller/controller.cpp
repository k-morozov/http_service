//
// Created by focus on 09.02.2022.
//

#include "gtest/gtest.h"

#include <common/cancel_controller/include/Controller.h>

#include <future>
#include <execution>

using namespace std::chrono_literals;

using future_t = std::future<void>;


class SuiteController : public ::testing::Test
{
public:
    SuiteController() = default;

protected:
    void SetUp() override {
        controller_ = sdk::Controller::create();
    }
    
    void TearDown() override {
        controller_.reset();
    }

    sdk::ControllerPtr controller_;
};

TEST_F(SuiteController, simpleCallHandler)
{
    ASSERT_TRUE(controller_) << "Failed allocate for controller";

    bool flag = false;
    auto f = [&flag]{ flag = true; };

    controller_->subscribe(std::move(f));
    EXPECT_FALSE(flag);
    EXPECT_FALSE(controller_->is_cancel());

    controller_->cancel();

    EXPECT_TRUE(controller_->is_cancel());
    EXPECT_TRUE(flag);
}

TEST_F(SuiteController, simpleCallHandlerInThreads)
{
    ASSERT_TRUE(controller_);

    std::atomic_uint counter = 0;
    auto f = [&counter]{ counter++; };

    controller_->subscribe(std::move(f));
    EXPECT_EQ(counter.load(), 0);
    EXPECT_FALSE(controller_->is_cancel());

    constexpr size_t COUNT_THREADS = 100;
    std::array<std::unique_ptr<future_t>, COUNT_THREADS> data;

    for(auto& p : data)
    {
        p = std::make_unique<future_t>(std::async(std::launch::async,
                                                            [c = controller_]{c->cancel();}
                                                            ));
        ASSERT_TRUE(p);
    }

    EXPECT_TRUE(controller_->is_cancel());
    std::for_each(std::execution::par_unseq, data.begin(), data.end(), [](auto&& p)
    {
        ASSERT_NO_THROW(p->get());
    });

    EXPECT_EQ(counter.load(), COUNT_THREADS);
}

TEST_F(SuiteController, simpleWait) {
    ASSERT_TRUE(controller_);

    auto f = [] {};

    controller_->subscribe(std::move(f));
    EXPECT_FALSE(controller_->is_cancel());

    controller_->start_work();

    constexpr size_t COUNT_THREADS = 100;
    std::array<std::unique_ptr<future_t>, COUNT_THREADS> data;

    auto job = []()
    {
        std::this_thread::sleep_for(2s);
    };

    std::atomic_uint counter = COUNT_THREADS;

    for(auto& p : data)
    {
        p = std::make_unique<future_t>(std::async(std::launch::async,
                                                  [c = controller_, job, &counter]
        {
            c->process(job);
            counter--;
        }
        ));
        ASSERT_TRUE(p);
    }

    controller_->end_work();
    controller_->wait();

    EXPECT_EQ(counter.load(), 0);

    std::for_each(std::execution::par_unseq, data.begin(), data.end(), [](auto&& p)
    {
        ASSERT_NO_THROW(p->get());
    });
}