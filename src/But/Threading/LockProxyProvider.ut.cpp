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


TEST_F(ButThreadingLockProxyProvider, ProxyLocksAndUnlocksObjectAutomatically)
{
  EXPECT_EQ( md_.locks_, 0 );
  {
    auto p = md_.withLock();
    (void)p;
    EXPECT_EQ( md_.locks_, 1 );
  }
  EXPECT_EQ( md_.locks_, 0 );
}


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


TEST_F(ButThreadingLockProxyProvider, ProxyIsMovable)
{
  EXPECT_TRUE( std::is_move_constructible<LockProxy<MyData>>::value );
  EXPECT_TRUE( std::is_move_assignable<LockProxy<MyData>>::value );
}


TEST_F(ButThreadingLockProxyProvider, MovingProxyDoesNotCauseExtraLockingNorUnlocking)
{
  EXPECT_TRUE( std::is_move_constructible<LockProxy<MyData>>::value );
  EXPECT_TRUE( std::is_move_assignable<LockProxy<MyData>>::value );

  EXPECT_EQ( md_.locks_, 0 );
  {
    auto p1 = md_.withLock();
    EXPECT_EQ( md_.locks_, 1 );
    auto p2 = std::move(p1);
    EXPECT_EQ( md_.locks_, 1 );
    p1 = std::move(p2);
    EXPECT_EQ( md_.locks_, 1 );
  }
  EXPECT_EQ( md_.locks_, 0 );
}


TEST_F(ButThreadingLockProxyProvider, MoveAsignmentReleasesPreviousLockWhenOwned)
{
  EXPECT_EQ( md_.locks_, 0 );
  MyData other;
  {
    auto p1 = other.withLock();
    auto p2 = md_.withLock();
    EXPECT_EQ( other.locks_, 1 );
    EXPECT_EQ( md_.locks_, 1 );
    p1 = std::move(p2);
    EXPECT_EQ( other.locks_, 0 );
    EXPECT_EQ( md_.locks_, 1 );
  }
  EXPECT_EQ( other.locks_, 0 );
  EXPECT_EQ( md_.locks_, 0 );
}


TEST_F(ButThreadingLockProxyProvider, LockSelfAssignmentIsNoOp)
{
  EXPECT_EQ( md_.locks_, 0 );
  {
    auto p1 = md_.withLock();
    EXPECT_EQ( md_.locks_, 1 );
    auto&& p2 = std::move(p1);
    p1 = std::move(p2);
    EXPECT_EQ( md_.locks_, 1 );
  }
  EXPECT_EQ( md_.locks_, 0 );
}


TEST_F(ButThreadingLockProxyProvider, ProxyIsNonCopyable)
{
  EXPECT_FALSE( std::is_copy_constructible<LockProxy<MyData>>::value );
  EXPECT_FALSE( std::is_copy_assignable<LockProxy<MyData>>::value );
}

}
