#include <string>
#include <chrono>

#include "gtest/gtest.h"
#include "ActiveObject.hpp"


namespace
{

struct ButThreadingActiveObject: public testing::Test
{
  But::Threading::ActiveObject ao_;
  std::chrono::seconds timeout_{5};
};


TEST_F(ButThreadingActiveObject, ClosingRightAway)
{
  // leave this empty
}


TEST_F(ButThreadingActiveObject, ProcessingIsRunningInSeparateThread)
{
  auto f = ao_.run( []{ return std::this_thread::get_id(); } );
  ASSERT_TRUE( f.wait_for(timeout_)==std::future_status::ready );
  EXPECT_TRUE( f.get() != std::this_thread::get_id() );
}

}
