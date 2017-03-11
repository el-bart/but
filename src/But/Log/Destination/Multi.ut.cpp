#include "gtest/gtest.h"
#include "Multi.hpp"

using But::Log::Destination::Multi;

namespace
{

template<int N>
struct TestDst final
{
  template<typename ...Args>
  void log(Args&& ...)
  {
    ++logs_;
    if(throws_)
      throw std::runtime_error{"throwing, as requested"};
  }

  void reload()
  {
    ++reloads_;
    if(throws_)
      throw std::runtime_error{"throwing, as requested"};
  }

  auto operator->() { return this; }

  bool throws_{false};
  unsigned logs_{0};
  unsigned reloads_{0};
};


struct ButLogDestinationMulti: public testing::Test
{
  TestDst<1> td1_;
  TestDst<2> td2_;
  TestDst<3> td3_;
  Multi<TestDst<1>*, TestDst<2>*, TestDst<3>*> multi_{&td1_, &td2_, &td3_};
};


TEST_F(ButLogDestinationMulti, SingleDestinationWorks)
{
  Multi<TestDst<1>*> m{&td1_};
  m.log("test");
  EXPECT_EQ( 1u, td1_.logs_ );
}


TEST_F(ButLogDestinationMulti, PrintingGoesThroughAllDestinations)
{
  multi_.log("one");
  EXPECT_EQ( 1u, td1_.logs_ );
  EXPECT_EQ( 1u, td2_.logs_ );
  EXPECT_EQ( 1u, td3_.logs_ );

  multi_.log("two");
  EXPECT_EQ( 2u, td1_.logs_ );
  EXPECT_EQ( 2u, td2_.logs_ );
  EXPECT_EQ( 2u, td3_.logs_ );
}


struct CopyMoveDst final
{
  void log(std::string const& str)
  {
    EXPECT_EQ( "data", str );
    ++copy_;
  }
  void log(std::string&& str)
  {
    EXPECT_EQ( "data", str );
    str.clear();
    ++move_;
  }

  unsigned copy_{0};
  unsigned move_{0};
};

TEST_F(ButLogDestinationMulti, ArgumentsArePassedToLastDestinationOnly)
{
  CopyMoveDst cp1;
  CopyMoveDst cp2;
  CopyMoveDst mv;
  Multi<CopyMoveDst*, CopyMoveDst*, CopyMoveDst*> multi{&cp1, &cp2, &mv};
  multi.log( std::string{"data"} );

  EXPECT_EQ( 1u, cp1.copy_ );
  EXPECT_EQ( 0u, cp1.move_ );

  EXPECT_EQ( 1u, cp2.copy_ );
  EXPECT_EQ( 0u, cp2.move_ );

  EXPECT_EQ( 0u, mv.copy_ );
  EXPECT_EQ( 1u, mv.move_ );
}


TEST_F(ButLogDestinationMulti, ExceptionInAnyPrinterDoesNotStopProcessing)
{
  td1_.throws_ = true;
  td2_.throws_ = true;
  td3_.throws_ = true;

  multi_.log("one");
  EXPECT_EQ( 1u, td1_.logs_ );
  EXPECT_EQ( 1u, td2_.logs_ );
  EXPECT_EQ( 1u, td3_.logs_ );
}


TEST_F(ButLogDestinationMulti, ReloadingReloadsAllDestinations)
{
}


TEST_F(ButLogDestinationMulti, ExceptionInAnyDestinationDoesNotStopProcessing)
{
}

}
