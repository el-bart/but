#include <type_traits>
#include <gtest/gtest.h>
#include <But/Threading/LockProxyProvider.hpp>

using But::Threading::LockProxyProvider;
using But::Threading::LockProxy;

namespace
{

struct MyData: LockProxyProvider<MyData>
{
  void lock()   const { ++locks_; }
  void unlock() const { --locks_; }

  auto locks() const { return locks_; }
  void constFoo() const { }
  void nonConstFoo() { }

  mutable int locks_{0};
};

struct ButThreadingLockProxyProvider: public testing::Test
{
  MyData md_;
};


TEST_F(ButThreadingLockProxyProvider, ArrowOperatorOnProxyForwardsToActualClass)
{
  EXPECT_EQ(md_.locks_, 0);
  const auto cnt = md_.withLock()->locks();
  EXPECT_EQ(cnt, 1);
  EXPECT_EQ(md_.locks_, 0);
}


TEST_F(ButThreadingLockProxyProvider, ArrowOperatorEnsureConstness)
{
  md_.withLock()->nonConstFoo();

  const auto& cmd = md_;
  cmd.withLock()->constFoo();
}

}
