#include "gtest/gtest.h"
#include "ThreadNo.hpp"
#include "But/Log/Backend/FieldInfo.hpp"
#include "But/Threading/JoiningThread.hpp"

using But::Log::Field::ThreadNo;

namespace
{

struct ButLogFieldThreadNo: public testing::Test
{ };


TEST_F(ButLogFieldThreadNo, ConvertingToString)
{
  EXPECT_GE( toString( ThreadNo{} ).length(), 1u );
}


TEST_F(ButLogFieldThreadNo, ConvertingToFieldInfo)
{
  const auto tno = ThreadNo{};
  const auto fi = But::Log::Backend::FieldInfo{tno};
  EXPECT_EQ( fi.type(), "But::ThreadNo" );
  EXPECT_EQ( toString(tno), fi.value() );
}


TEST_F(ButLogFieldThreadNo, TheSameThreadReturnsTheSameNumber)
{
  const auto tno1 = ThreadNo{};
  const auto tno2 = ThreadNo{};
  EXPECT_EQ( tno1.value_, tno2.value_ );
}


TEST_F(ButLogFieldThreadNo, NewlyCreatedThreadsGetConsecutiveNumbers)
{
  const auto tno1 = ThreadNo{};

  ThreadNo tno2;
  std::thread{ [&] { tno2 = ThreadNo{}; } }.join();
  EXPECT_EQ( tno1.value_ + 1u, tno2.value_ );

  ThreadNo tno3;
  std::thread{ [&] { tno3 = ThreadNo{}; } }.join();
  EXPECT_EQ( tno2.value_ + 1u, tno3.value_ );
}

}
