#include <string>
#include <chrono>
#include <gtest/gtest.h>
#include <But/Threading/ActiveObject.hpp>
#include <But/Threading/detail/waitForFuture.ut.hpp>
#include <But/gtest_compat.ut.hpp>

using But::Threading::detail::waitForFuture;

namespace
{

struct ButThreadingActiveObject: public testing::Test
{
  But::Threading::ActiveObject ao_;
};


TEST_F(ButThreadingActiveObject, ClosingRightAway)
{
  // leave this empty
}


TEST_F(ButThreadingActiveObject, ProcessingIsRunningInSeparateThread)
{
  auto f = this->ao_.run( []{ return std::this_thread::get_id(); } );
  ASSERT_TRUE( waitForFuture(f) );
  EXPECT_TRUE( f.get() != std::this_thread::get_id() );
}

}
