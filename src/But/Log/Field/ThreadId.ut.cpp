#include "gtest/gtest.h"
#include "ThreadId.hpp"
#include "But/Log/Backend/FieldInfo.hpp"
#include "But/Threading/JoiningThread.hpp"

using But::Log::Backend::Type;
using But::Log::Field::ThreadId;

namespace
{

struct ButLogFieldThreadId: public testing::Test
{ };


TEST_F(ButLogFieldThreadId, ConvertingToString)
{
  EXPECT_GT( toValue( ThreadId{} ).get<std::string>().length(), 4u );
}


TEST_F(ButLogFieldThreadId, ConvertingToFieldInfo)
{
  const auto tid = ThreadId{};
  const auto fi = But::Log::Backend::FieldInfo{tid};
  EXPECT_EQ( fi.type(), Type{"But::ThreadId"} );
  EXPECT_EQ( toValue(tid), fi.value() );
}


TEST_F(ButLogFieldThreadId, TheSameThreadReturnsTheSameId)
{
  const auto tid1 = ThreadId{};
  const auto tid2 = ThreadId{};
  EXPECT_EQ( tid1.value_, tid2.value_ );
}


TEST_F(ButLogFieldThreadId, DefaultCtorGetsThreadIdOfTheCurrentThread)
{
  EXPECT_TRUE( ThreadId{}.value_ == std::this_thread::get_id() );
}


TEST_F(ButLogFieldThreadId, DifferentThreadsHaveDifferentIds)
{
  const auto tid1 = ThreadId{};
  ThreadId tid2;
  std::thread{ [&] { tid2 = ThreadId{}; } }.join();
  EXPECT_NE( tid1.value_, tid2.value_ );
}

}
