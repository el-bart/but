#include <gtest/gtest.h>
#include <But/System/CpuTimer.hpp>

using But::System::CpuTimer;

namespace
{

struct ButSystemCpuTimer: public testing::Test
{
};


TEST_F(ButSystemCpuTimer, TestIfValueIsStrictlyIncremental)
{
  for(auto i=0; i<100; ++i)  // we may need multiple iterations, due to cpu scheduling
  {
    CpuTimer dt;
    const auto start = dt.elapsed();
    if(not start)
      continue;
    EXPECT_LT(0u, *start);
    const auto stop = dt.elapsed();
    if(not stop)
      continue;
    EXPECT_LT(0u, *start);
    EXPECT_LT(*start, *stop);
  }
}

}
