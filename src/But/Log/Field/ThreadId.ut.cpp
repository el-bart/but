#include <sstream>
#include "gtest/gtest.h"
#include "ThreadId.hpp"
#include "But/Threading/JoiningThread.hpp"

using But::Log::Backend::Tag;
using But::Log::Field::ThreadId;

namespace
{

struct ButLogFieldThreadId: public testing::Test
{ };


TEST_F(ButLogFieldThreadId, ConvertingToFieldInfo)
{
  const auto tid = ThreadId{};
  const auto fi = toFieldInfo(tid);
  EXPECT_EQ( Tag{"But::ThreadId"}, fi.type() );
  std::stringstream ss;
  ss << tid.value_;
  EXPECT_EQ( ss.str(), fi.value().get<std::string>() );
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
