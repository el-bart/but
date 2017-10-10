#include "gtest/gtest.h"
#include "Pid.hpp"

using But::Log::Backend::Tag;
using But::Log::Field::Pid;

namespace
{

struct ButLogFieldPid: public testing::Test
{ };


TEST_F(ButLogFieldPid, ConvertingToFieldInfo)
{
  const auto pid = Pid{};
  const auto fi = toFieldInfo(pid);
  EXPECT_EQ( Tag{"But::Pid"}, fi.type() );
  EXPECT_EQ( pid.value_, fi.value().get<int64_t>() );
}


TEST_F(ButLogFieldPid, PidDoesNotChange)
{
  const auto pid1 = Pid{};
  const auto pid2 = Pid{};
  EXPECT_EQ( pid1.value_, pid2.value_ );
}


TEST_F(ButLogFieldPid, DefaultCtorGetsPidOfTheCurrentProcess)
{
  EXPECT_EQ( getpid(), Pid{}.value_ );
}

}
