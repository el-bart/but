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
  Q q_;
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

}
