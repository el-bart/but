#include <sstream>
#include <But/Log/Field/ThreadId.hpp>
#include <But/Threading/JoiningThread.hpp>
#include <But/Log/Backend/detail/EntryRoot.ut.hpp>

using But::Log::Field::ThreadId;

namespace
{

struct ButLogFieldThreadId: public But::Log::Backend::detail::EntryRootTestBase
{ };


TEST_F(ButLogFieldThreadId, ConvertingToFieldInfo)
{
  const auto tid = ThreadId{};
  EXPECT_EQ( "But::ThreadId", fieldName(&tid) );
  std::stringstream ss;
  ss << tid.value_;
  EXPECT_EQ( ss.str(), fieldValue(tid) );
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

TEST_F(ButLogFieldThreadId, Logging)
{
  const auto tid = ThreadId{};
  er_.proxy().nest(tid);
  std::stringstream ss;
  ss << tid.value_;
  EXPECT_EQ_JSON("{ \"But::ThreadId\": \"" + ss.str() + "\" }", er_);
}

}
