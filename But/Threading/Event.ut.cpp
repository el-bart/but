#include <thread>

#include "gtest/gtest.h"
#include "Event.hpp"
#include "JoiningThread.hpp"

using But::Threading::Event;

namespace
{

using Thread = But::Threading::JoiningThread<std::thread>;

struct ButThreadingEvent: public ::testing::Test
{
  Event e_;
};


TEST_F(ButThreadingEvent, SetUnblocks)
{
  e_.set();
  e_.wait();    // does not block
}


TEST_F(ButThreadingEvent, WaitIsConst)
{
  e_.set();
  static_cast<Event const&>(e_).wait();
}


TEST_F(ButThreadingEvent, BlockedByDefault)
{
  std::string v{"whatever"};
  Thread th{ [&]{ v = "ok"; e_.set(); } };
  e_.wait();
  EXPECT_EQ(v, "ok");
}


TEST_F(ButThreadingEvent, BlockedWithTimeout)
{
  std::string v{"whatever"};
  Thread th{ [&]{ v = "ok"; e_.set(); } };
  e_.wait(std::chrono::seconds{5});
  EXPECT_EQ(v, "ok");
}


TEST_F(ButThreadingEvent, BlockedWithDeadline)
{
  std::string v{"whatever"};
  Thread th{ [&]{ v = "ok"; e_.set(); } };
  e_.wait(std::chrono::steady_clock::now() + std::chrono::seconds{5});
  EXPECT_EQ(v, "ok");
}


TEST_F(ButThreadingEvent, UnblockedCanBeBlockedAgain)
{
  e_.set();
  e_.wait();
  e_.clear();
  std::string v{"whatever"};
  Thread th{ [&]{ v = "ok"; e_.set(); } };
  e_.wait(std::chrono::seconds{5});
  EXPECT_EQ(v, "ok");
}

}
