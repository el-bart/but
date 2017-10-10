#include "gtest/gtest.h"
#include "ThreadNo.hpp"
#include "But/Threading/JoiningThread.hpp"

using But::Log::Backend::Tag;
using But::Log::Backend::Value;
using But::Log::Field::ThreadNo;

namespace
{

struct ButLogFieldThreadNo: public testing::Test
{ };


TEST_F(ButLogFieldThreadNo, ConvertingToFieldInfo)
{
  const auto tno = ThreadNo{};
  const auto fi = toFieldInfo(tno);
  EXPECT_EQ( Tag{"But::ThreadNo"}, fi.tag() );
  EXPECT_EQ( Value{tno.value_}, fi.value() );
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
