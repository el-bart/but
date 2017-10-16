#include <string>
#include <atomic>
#include <memory>
#include <thread>
#include <gtest/gtest.h>
#include <But/Threading/Fifo.hpp>
#include <But/Threading/JoiningThread.hpp>

using But::Threading::Fifo;
using Thread = But::Threading::JoiningThread<std::thread>;

namespace
{

using Q = Fifo<std::string>;

struct ButThreadingFifo: public testing::Test
{
  Q                                     q_;
  std::chrono::seconds                  timeout_{5};
  std::chrono::steady_clock::time_point deadline_{std::chrono::steady_clock::now() + timeout_};
};


TEST_F(ButThreadingFifo, DoNothing)
{
}


TEST_F(ButThreadingFifo, PushPopOne)
{
  Q::lock_type lock{q_};
  EXPECT_TRUE(q_.empty());

  q_.push("test");
  EXPECT_FALSE(q_.empty());
  ASSERT_EQ(q_.size(), 1u);
  EXPECT_EQ(q_.top(), "test");

  q_.pop();
  EXPECT_EQ(q_.size(), 0u);
  EXPECT_TRUE(q_.empty());
}


TEST_F(ButThreadingFifo, Order)
{
  Q::lock_type lock{q_};
  EXPECT_TRUE(q_.empty());

  q_.push("1");
  q_.push("2");
  q_.push("3");

  ASSERT_EQ(q_.size(), 3u);
  EXPECT_EQ(q_.top(), "1");
  q_.pop();

  ASSERT_EQ(q_.size(), 2u);
  EXPECT_EQ(q_.top(), "2");
  q_.pop();

  ASSERT_EQ(q_.size(), 1u);
  EXPECT_EQ(q_.top(), "3");
  q_.pop();

  EXPECT_EQ(q_.size(), 0u);
  EXPECT_TRUE(q_.empty());
}


TEST_F(ButThreadingFifo, TopConst)
{
  Q::lock_type lock{q_};
  EXPECT_TRUE(q_.empty());

  q_.push("test");
  ASSERT_EQ(q_.size(), 1u);
  EXPECT_EQ( static_cast<Q const&>(q_).top(), "test");
}


TEST_F(ButThreadingFifo, MovableOnlyObjects)
{
  Fifo<std::unique_ptr<int>> q;
  decltype(q)::lock_type lock{q};
  EXPECT_TRUE(q.empty());

  q.push(std::make_unique<int>(42));
  EXPECT_FALSE(q.empty());
  ASSERT_EQ(q.size(), 1u);
  std::unique_ptr<int> p = std::move(q.top());
  ASSERT_TRUE(p.get()!=nullptr);
  EXPECT_EQ(*p, 42);

  q.pop();
  EXPECT_EQ(q.size(), 0u);
  EXPECT_TRUE(q.empty());
}


TEST_F(ButThreadingFifo, WaitForElementAdditionWithTimeout)
{
  Thread th{ [&]{ Q::lock_type lock{q_}; q_.push("narf"); } };
  Q::lock_type lock{q_};
  ASSERT_TRUE( q_.waitForNonEmpty(lock, timeout_) );
  EXPECT_EQ( "narf", q_.top() );
}


TEST_F(ButThreadingFifo, WaitForElementAddition)
{
  Thread th{ [&]{ Q::lock_type lock{q_}; q_.push("narf"); } };
  Q::lock_type lock{q_};
  ASSERT_TRUE( q_.waitForNonEmpty(lock) );
  EXPECT_EQ( "narf", q_.top() );
}


TEST_F(ButThreadingFifo, WaitForElementRemovalWithTimeout)
{
  Thread th{ [&]{ Q::lock_type lock{q_}; q_.push("narf"); } };
  Q::lock_type lock{q_};
  ASSERT_TRUE( q_.waitForNonEmpty(lock, timeout_) );
  q_.pop();
}


TEST_F(ButThreadingFifo, WaitForElementRemoval)
{
  Thread th{ [&]{ Q::lock_type lock{q_}; q_.push("narf"); } };
  Q::lock_type lock{q_};
  ASSERT_TRUE( q_.waitForNonEmpty(lock) );
  q_.pop();
}


TEST_F(ButThreadingFifo, NonEmptyNoTimeoutWhenElementIsPresent)
{
  Q::lock_type lock{q_};
  q_.push("test");
  ASSERT_EQ(q_.size(), 1u);

  EXPECT_TRUE( q_.waitForNonEmpty(lock, timeout_) );
  EXPECT_TRUE( q_.waitForNonEmpty(lock, deadline_) );

  auto const& cq = q_;
  EXPECT_TRUE( cq.waitForNonEmpty(lock, timeout_) );
  EXPECT_TRUE( cq.waitForNonEmpty(lock, deadline_) );
}

TEST_F(ButThreadingFifo, NonEmptyTimeoutWhenElementIsNotPresent)
{
  Q::lock_type lock{q_};
  ASSERT_TRUE( q_.empty() );
  const auto timeout = std::chrono::seconds{0};
  const auto deadline = std::chrono::steady_clock::now();

  EXPECT_FALSE( q_.waitForNonEmpty(lock, timeout) );
  EXPECT_FALSE( q_.waitForNonEmpty(lock, deadline) );

  auto const& cq = q_;
  EXPECT_FALSE( cq.waitForNonEmpty(lock, timeout) );
  EXPECT_FALSE( cq.waitForNonEmpty(lock, deadline) );
}


TEST_F(ButThreadingFifo, ProducerConsumer)
{
  const auto count = 1000;
  Thread th{ [&]{ for(int i=0; i<count; ++i) { Q::lock_type lock{q_}; q_.push("foo/bar"); std::this_thread::yield(); } } };
  const auto deadline = std::chrono::steady_clock::now() + 2*timeout_;
  for(int i=0; i<count; ++i)
  {
    Q::lock_type lock{q_};
    ASSERT_TRUE( q_.waitForNonEmpty(lock, deadline) );
    EXPECT_EQ( q_.top(), "foo/bar" );
    q_.pop();
    std::this_thread::yield();
  }
}


TEST_F(ButThreadingFifo, ExplicitWaitingForAddition)
{
  std::atomic<bool> finished{false};
  Thread th{ [&] {
                   Q::lock_type lock{q_};
                   ASSERT_TRUE( q_.waitForNonEmpty(lock) );
                   finished = true;
                 } };

  Q::lock_type lock{q_};
  EXPECT_FALSE(finished);
  q_.push("sth");
}


TEST_F(ButThreadingFifo, ExplicitWaitingForAdditionWithTimeout)
{
  std::atomic<bool> finished{false};
  Thread th{ [&] {
                   Q::lock_type lock{q_};
                   ASSERT_TRUE( q_.waitForNonEmpty(lock, timeout_) );
                   finished = true;
                 } };

  Q::lock_type lock{q_};
  q_.push("sth");
}


TEST_F(ButThreadingFifo, ExplicitWaitingForRemoval)
{
  {
    Q::lock_type lock{q_};
    q_.push("1");
    q_.push("2");
  }

  Thread th{ [&] { Q::lock_type lock{q_}; q_.pop(); } };

  Q::lock_type lock{q_};
  EXPECT_TRUE( q_.waitForSizeBelow(2, lock, timeout_) );
  EXPECT_EQ( 1u, q_.size() );
}


TEST_F(ButThreadingFifo, ExplicitWaitingForRemovalWithZeroSizeAlwaysSucceeds)
{
  Q::lock_type lock{q_};
  EXPECT_EQ( 0u, q_.size() );
  EXPECT_TRUE( q_.waitForSizeBelow(0, lock, timeout_) );
  EXPECT_EQ( 0u, q_.size() );
}

}
