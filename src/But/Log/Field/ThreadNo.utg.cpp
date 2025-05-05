#include <But/Log/Field/ThreadNo.hpp>
#include <But/Threading/JoiningThread.hpp>
#include <But/Log/Backend/EntryRootTestBase.utg.hpp>

using But::Log::Field::ThreadNo;

namespace
{

struct ButLogFieldThreadNo: public But::Log::Backend::EntryRootTestBase
{ };


TEST_F(ButLogFieldThreadNo, ConvertingToFieldInfo)
{
  const auto tno = ThreadNo{};
  EXPECT_EQ( "But::ThreadNo", fieldName(&tno) );
  EXPECT_EQ( tno.value_, fieldValue(tno) );
}


TEST_F(ButLogFieldThreadNo, Logging)
{
  auto tno = ThreadNo{};
  tno.value_ = 42;
  er_.proxy().nest(tno);
  EXPECT_EQ_JSON(R"({ "But::ThreadNo": 42 })", er_);
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
