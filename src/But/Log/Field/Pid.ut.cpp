#include <But/Log/Field/Pid.hpp>
#include <But/Log/Backend/detail/EntryRoot.ut.hpp>

using But::Log::Field::Pid;

namespace
{

struct ButLogFieldPid: public But::Log::Backend::detail::EntryRootTestBase
{ };


TEST_F(ButLogFieldPid, NameAndValue)
{
  Pid p;
  p.value_ = 13;
  EXPECT_EQ( fieldName(&p), "But::Pid" );
  EXPECT_EQ( fieldValue(p), 13 );
}


TEST_F(ButLogFieldPid, Logging)
{
  Pid p;
  p.value_ = 42;
  er_.proxy().nest(p);
  EXPECT_EQ_JSON(R"({ "But::Pid": 42 })", er_);
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
