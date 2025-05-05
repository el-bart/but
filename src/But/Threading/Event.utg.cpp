#include <thread>
#include <gtest/gtest.h>
#include <But/Threading/Event.hpp>
#include <But/Threading/JoiningThread.hpp>
#include <But/detail/tsan.hpp>

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
  EXPECT_TRUE( e_.wait() );     // does not block
}


TEST_F(ButThreadingEvent, WaitIsConst)
{
  e_.set();
  auto const& ce = e_;
  EXPECT_TRUE( ce.wait() );
}


TEST_F(ButThreadingEvent, BlockedByDefault)
{
  std::string v{"whatever"};
  Thread th{ [&]{ v = "ok"; e_.set(); } };
  ASSERT_TRUE( e_.wait() );
  EXPECT_EQ(v, "ok");
}

#ifdef BUT_THREAD_SANITIZER_WORKAROUND_NEEDED

TEST_F(ButThreadingEvent, BlockedWithTimeout)
{
  std::string v{"whatever"};
  Thread th{ [&]{ v = "ok"; e_.set(); } };
  ASSERT_TRUE( e_.wait( std::chrono::seconds{5} ) );
  EXPECT_EQ(v, "ok");
}


TEST_F(ButThreadingEvent, BlockedWithDeadline)
{
  std::string v{"whatever"};
  Thread th{ [&]{ v = "ok"; e_.set(); } };
  ASSERT_TRUE( e_.wait( std::chrono::steady_clock::now() + std::chrono::seconds{5} ) );
  EXPECT_EQ(v, "ok");
}


TEST_F(ButThreadingEvent, UnblockedCanBeBlockedAgain)
{
  e_.set();
  e_.wait();
  e_.clear();
  std::string v{"whatever"};
  Thread th{ [&]{ v = "ok"; e_.set(); } };
  ASSERT_TRUE( e_.wait( std::chrono::seconds{5} ) );
  EXPECT_EQ(v, "ok");
}


TEST_F(ButThreadingEvent, TimeoutOnBlocking)
{
  EXPECT_FALSE( e_.wait( std::chrono::steady_clock::now() + std::chrono::seconds{0} ) );
  EXPECT_FALSE( e_.wait( std::chrono::seconds{0} ) );
}

#endif // BUT_THREAD_SANITIZER_WORKAROUND_NEEDED

}
