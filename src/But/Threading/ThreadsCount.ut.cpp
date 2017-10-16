#include <gtest/gtest.h>
#include <But/Threading/ThreadsCount.hpp>

using But::Threading::ThreadsCount;

namespace
{

struct ButThreadingThreadsCount: public testing::Test
{ };


TEST_F(ButThreadingThreadsCount, PositiveNumberOfThreads)
{
  EXPECT_EQ( 1u, ThreadsCount{1}.count() );
  EXPECT_EQ( 2u, ThreadsCount{2}.count() );
}


TEST_F(ButThreadingThreadsCount, ErrorOnZeroThreads)
{
  EXPECT_THROW( ThreadsCount{0}, ThreadsCount::InvalidNumberOfThreads );
}

}
