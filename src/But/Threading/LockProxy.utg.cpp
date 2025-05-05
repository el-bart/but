#include <type_traits>
#include <gtest/gtest.h>
#include <But/Threading/LockProxy.hpp>

using But::Threading::LockProxy;

namespace
{

struct MyData
{
  void lock()   const { ++locks_; }
  void unlock() const { --locks_; }

  auto locks() const { return locks_; }
  void constFoo() const { }
  void nonConstFoo() { }

  mutable int locks_{0};
};

struct ButThreadingLockProxy: public testing::Test
{
  MyData md_;
};

using Proxy = LockProxy<MyData>;


TEST_F(ButThreadingLockProxy, ProxyLocksAndUnlocksObjectAutomatically)
{
  EXPECT_EQ( md_.locks_, 0 );
  {
    Proxy lp{md_};
    EXPECT_EQ( md_.locks_, 1 );
  }
  EXPECT_EQ( md_.locks_, 0 );
}


TEST_F(ButThreadingLockProxy, ArrowOperatorOnProxyForwardsToActualClass)
{
  EXPECT_EQ(md_.locks_, 0);
  Proxy lp{md_};
  EXPECT_EQ( 1, lp->locks() );
}


TEST_F(ButThreadingLockProxy, ArrowOperatorEnsureConstness)
{
  Proxy lp{md_};
  lp->nonConstFoo();

  const LockProxy<const MyData> clp{md_};
  clp->constFoo();
}


TEST_F(ButThreadingLockProxy, GetMemberFunctionEnsureConstness)
{
  Proxy lp{md_};
  lp.get()->nonConstFoo();

  const LockProxy<const MyData> clp{md_};
  clp.get()->constFoo();
}


TEST_F(ButThreadingLockProxy, DereferenceOperatorOnProxyForwardsToActualClass)
{
  EXPECT_EQ(md_.locks_, 0);
  Proxy lp{md_};
  EXPECT_EQ( 1, (*lp).locks() );
}


TEST_F(ButThreadingLockProxy, DereferenceOperatorEnsureConstness)
{
  Proxy lp{md_};
  (*lp).nonConstFoo();

  const LockProxy<const MyData> clp{md_};
  (*clp).constFoo();
}


TEST_F(ButThreadingLockProxy, ProxyIsMovable)
{
  EXPECT_TRUE( std::is_move_constructible<Proxy>::value );
  EXPECT_TRUE( std::is_move_assignable<Proxy>::value );
}


TEST_F(ButThreadingLockProxy, ProxyIsNonCopyable)
{
  EXPECT_FALSE( std::is_copy_constructible<Proxy>::value );
  EXPECT_FALSE( std::is_copy_assignable<Proxy>::value );
}


TEST_F(ButThreadingLockProxy, MovingProxyDoesNotCauseExtraLockingNorUnlocking)
{
  EXPECT_EQ( md_.locks_, 0 );
  {
    Proxy p1{md_};
    EXPECT_EQ( md_.locks_, 1 );
    auto p2 = std::move(p1);
    EXPECT_EQ( md_.locks_, 1 );
    p1 = std::move(p2);
    EXPECT_EQ( md_.locks_, 1 );
  }
  EXPECT_EQ( md_.locks_, 0 );
}


TEST_F(ButThreadingLockProxy, MoveAsignmentReleasesPreviousLockWhenOwned)
{
  EXPECT_EQ( md_.locks_, 0 );
  MyData other;
  {
    Proxy p1{other};
    Proxy p2{md_};
    EXPECT_EQ( other.locks_, 1 );
    EXPECT_EQ( md_.locks_, 1 );
    p1 = std::move(p2);
    EXPECT_EQ( other.locks_, 0 );
    EXPECT_EQ( md_.locks_, 1 );
  }
  EXPECT_EQ( other.locks_, 0 );
  EXPECT_EQ( md_.locks_, 0 );
}


TEST_F(ButThreadingLockProxy, LockSelfAssignmentIsNoOp)
{
  EXPECT_EQ( md_.locks_, 0 );
  {
    Proxy p1{md_};
    EXPECT_EQ( md_.locks_, 1 );
    auto&& p2 = std::move(p1);
    p1 = std::move(p2);
    EXPECT_EQ( md_.locks_, 1 );
  }
  EXPECT_EQ( md_.locks_, 0 );
}

}
