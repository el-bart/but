#include <string>
#include <memory>
#include <thread>

#include "gtest/gtest.h"
#include "Fifo.hpp"
#include "JoiningThread.hpp"

using But::Threading::Fifo;
using Thread = But::Threading::JoiningThread<std::thread>;

namespace
{

using Q = Fifo<std::string>;

struct ThreadingFifo: public testing::Test
{
  Q                                     q_;
  std::chrono::seconds                  timeout_{5};
  std::chrono::steady_clock::time_point deadline_{std::chrono::steady_clock::now() + timeout_};
};


TEST_F(ThreadingFifo, DoNothing)
{
}


TEST_F(ThreadingFifo, PushPopOne)
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


TEST_F(ThreadingFifo, Order)
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


TEST_F(ThreadingFifo, TopConst)
{
  Q::lock_type lock{q_};
  EXPECT_TRUE(q_.empty());

  q_.push("test");
  ASSERT_EQ(q_.size(), 1u);
  EXPECT_EQ( static_cast<Q const&>(q_).top(), "test");
}


TEST_F(ThreadingFifo, MovableOnlyObjects)
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


TEST_F(ThreadingFifo, TopWaitForElementToTop)
{
  Thread th{ [&]{ Q::lock_type lock{q_}; q_.push("narf"); } };
  Q::lock_type lock{q_};
  EXPECT_EQ( q_.top(lock), "narf" );
}


TEST_F(ThreadingFifo, TopWaitForElementToTopConst)
{
  Thread th{ [&]{ Q::lock_type lock{q_}; q_.push("narf"); } };
  Q::lock_type lock{q_};
  EXPECT_EQ( static_cast<Q const&>(q_).top(lock), "narf" );
}


TEST_F(ThreadingFifo, TopWaitForElementToPop)
{
  Thread th{ [&]{ Q::lock_type lock{q_}; q_.push("narf"); } };
  Q::lock_type lock{q_};
  q_.pop(lock);
}


TEST_F(ThreadingFifo, TopWithTimeoutWhenElementIsPresent)
{
  Q::lock_type lock{q_};
  q_.push("test");
  ASSERT_EQ(q_.size(), 1u);
  EXPECT_EQ( q_.top(lock, timeout_), "test");
  EXPECT_EQ( q_.top(lock, deadline_), "test");
  EXPECT_EQ( static_cast<Q const&>(q_).top(lock, timeout_), "test");
  EXPECT_EQ( static_cast<Q const&>(q_).top(lock, deadline_), "test");
}


TEST_F(ThreadingFifo, TopTimeout)
{
  Q::lock_type lock{q_};
  timeout_  = std::chrono::seconds{0};
  deadline_ = std::chrono::steady_clock::now();
  EXPECT_THROW( q_.top(lock, timeout_),  Q::Timeout );
  EXPECT_THROW( q_.top(lock, deadline_), Q::Timeout );
  EXPECT_THROW( static_cast<Q const&>(q_).top(lock, timeout_),  Q::Timeout );
  EXPECT_THROW( static_cast<Q const&>(q_).top(lock, deadline_), Q::Timeout );
}


TEST_F(ThreadingFifo, PopWithTimeoutWhenElementIsPresent)
{
  Q::lock_type lock{q_};
  q_.push("test");
  q_.push("test");
  ASSERT_EQ(q_.size(), 2u);
  q_.pop(lock, timeout_);
  ASSERT_EQ(q_.size(), 1u);
  q_.pop(lock, deadline_);
  ASSERT_EQ(q_.size(), 0u);
}


TEST_F(ThreadingFifo, PopTimeout)
{
  Q::lock_type lock{q_};
  timeout_  = std::chrono::seconds{0};
  deadline_ = std::chrono::steady_clock::now();
  EXPECT_THROW( q_.pop(lock, timeout_),  Q::Timeout );
  EXPECT_THROW( q_.pop(lock, deadline_), Q::Timeout );
}


TEST_F(ThreadingFifo, ProducerConsumer)
{
  const auto count = 1000;
  Thread th{ [&]{ for(int i=0; i<count; ++i) { Q::lock_type lock{q_}; q_.push("foo/bar"); std::this_thread::yield(); } } };
  const auto deadline = std::chrono::steady_clock::now() + 2*timeout_;
  for(int i=0; i<count; ++i)
  {
    Q::lock_type lock{q_};
    EXPECT_EQ( q_.top(lock, deadline), "foo/bar" );
    q_.pop();
    std::this_thread::yield();
  }
}

}
