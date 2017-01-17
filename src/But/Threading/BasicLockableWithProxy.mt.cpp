#include "gtest/gtest.h"
#include "BasicLockable.hpp"
#include "LockProxyProvider.hpp"

using But::Threading::BasicLockable;
using But::Threading::LockProxyProvider;

namespace
{

struct MyData: BasicLockable<MyData>,
               LockProxyProvider<MyData>
{
  void doStuff() { }
  void doConstStuff() const { }
};

struct ButThreadingBasicLockableWithProxy: public testing::Test
{
  MyData md_;
};


TEST_F(ButThreadingBasicLockableWithProxy, IsLockable)
{
  const std::lock_guard<MyData> lock{md_};
  md_.doStuff();
  md_.doConstStuff();
}


TEST_F(ButThreadingBasicLockableWithProxy, WithLockWorks)
{
  auto proxy = md_.withLock();
  proxy->doStuff();
  proxy->doConstStuff();
}


TEST_F(ButThreadingBasicLockableWithProxy, WithLockWorksOnTemporaryObjects)
{
  md_.withLock()->doStuff();
  md_.withLock()->doConstStuff();
}

}
