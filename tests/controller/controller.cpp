//
// Created by focus on 09.02.2022.
//

#include "gtest/gtest.h"

#include <common/cancel_controller/include/Controller.h>

#include <future>
#include <execution>


class TestContext : public ::testing::Test
{
public:
    TestContext() = default;
};

TEST_F(TestContext, simpleCallHandler)
{
    auto controller_ = sdk::Controller::create();
    ASSERT_TRUE(controller_);

    bool flag = false;
    auto f = [&flag]{ flag = true; };

    controller_->subscribe(std::move(f));
    EXPECT_FALSE(flag);
    EXPECT_FALSE(controller_->is_cancel());

    controller_->cancel();

    EXPECT_TRUE(controller_->is_cancel());
    EXPECT_TRUE(flag);
}

TEST_F(TestContext, simpleCallHandlerInThreads)
{
    auto controller_ = sdk::Controller::create();
    ASSERT_TRUE(controller_);

    std::atomic_uint counter = 0;
    auto f = [&counter]{ counter++; };

    controller_->subscribe(std::move(f));
    EXPECT_EQ(counter.load(), 0);
    EXPECT_FALSE(controller_->is_cancel());

    constexpr size_t COUNT_THREADS = 100;
    std::array<std::unique_ptr<std::future<void>>, COUNT_THREADS> data;

    for(auto& p : data)
    {
        p = std::make_unique<std::future<void>>(std::async(std::launch::async,
                                                            [&controller_]{controller_->cancel();}
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