#include "gtest/gtest.h"
#include "Pid.hpp"
#include "But/Log/Backend/FieldInfo.hpp"

using But::Log::Field::Pid;

namespace
{

struct ButLogFieldPid: public testing::Test
{ };


TEST_F(ButLogFieldPid, ConvertingToString)
{
  EXPECT_EQ( getpid(), toValue( Pid{} ).get<int64_t>() );
}


TEST_F(ButLogFieldPid, ConvertingToFieldInfo)
{
  const auto pid = Pid{};
  const auto fi = But::Log::Backend::FieldInfo{pid};
  EXPECT_EQ( "But::Pid", fi.type() );
  EXPECT_EQ( toValue(pid), fi.value() );
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
