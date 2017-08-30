#include <thread>
#include <string>
#include "gtest/gtest.h"
#include "BasicLockable.hpp"
#include "JoiningThread.hpp"

using But::Threading::BasicLockable;

namespace
{

struct MyData: BasicLockable<MyData>
{
#ifndef NDEBUG
  bool lockedFwd() const { return locked(); }
#endif

  std::string str_;
};

struct ButThreadingBasicLockable: public testing::Test
{
  MyData md_;
};


TEST_F(ButThreadingBasicLockable, LockUnlockLoopDoesNotBlock)
{
  for(auto i=0; i<10; ++i)
    const std::lock_guard<MyData> lock{md_};
}


void lockTest(MyData& md)
{
  for(auto i=0; i<1000; ++i)
  {
    const std::lock_guard<MyData> lock{md};
    md.str_ += std::to_string(i) + "\n";
    std::this_thread::yield();
  }
}

TEST_F(ButThreadingBasicLockable, LockingWorks)
{
  {
    using Thread = But::Threading::JoiningThread<std::thread>;
    Thread th1{ lockTest, std::ref(md_) };
    Thread th2{ lockTest, std::ref(md_) };
    Thread th3{ lockTest, std::ref(md_) };
  }
  const auto newLines = std::count( md_.str_.cbegin(), md_.str_.cend(), '\n' );
  EXPECT_EQ( newLines, 3*1000 );
}


TEST_F(ButThreadingBasicLockable, DerivedClassCanCheckIfLocked)
{
  BUT_ASSERT( not md_.lockedFwd() );
  {
    const std::lock_guard<MyData> lock{md_};
    BUT_ASSERT( md_.lockedFwd() );
  }
  BUT_ASSERT( not md_.lockedFwd() );
}


TEST_F(ButThreadingBasicLockable, LockingUnlockingAndCheckingCanBeDoneOnConstObject)
{
  const auto& md = md_;
  const std::lock_guard<const MyData> lock{md};
  BUT_ASSERT( md.lockedFwd() );
}

}
