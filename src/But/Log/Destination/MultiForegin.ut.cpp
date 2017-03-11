#include "gtest/gtest.h"
#include "MultiForegin.hpp"

using But::Log::Backend::Entry;
using But::Log::Destination::Foregin;
using But::Log::Destination::MultiForegin;

namespace
{

template<int N>
struct TestDst final: public Foregin
{
  void logImpl(Entry) override
  {
    ++logs_;
    if(throws_)
      throw std::runtime_error{"throwing, as requested"};
  }

  void reloadImpl() override
  {
    ++reloads_;
    if(throws_)
      throw std::runtime_error{"throwing, as requested"};
  }

  bool throws_{false};
  unsigned logs_{0};
  unsigned reloads_{0};
};


struct ButLogDestinationMultiForegin: public testing::Test
{
  std::shared_ptr<TestDst<1>> td1_{ std::make_shared<TestDst<1>>() };
  std::shared_ptr<TestDst<2>> td2_{ std::make_shared<TestDst<2>>() };
  std::shared_ptr<TestDst<3>> td3_{ std::make_shared<TestDst<3>>() };
  MultiForegin multi_{ {td1_, td2_, td3_} };
};


TEST_F(ButLogDestinationMultiForegin, NoDestinationsSmokeTest)
{
  MultiForegin m{ {} };
  m.log("test");
}


TEST_F(ButLogDestinationMultiForegin, SingleDestinationWorks)
{
  MultiForegin m{ {td1_} };
  m.log("test");
  EXPECT_EQ( 1u, td1_->logs_ );
}


TEST_F(ButLogDestinationMultiForegin, PrintingGoesThroughAllDestinations)
{
  multi_.log("one");
  EXPECT_EQ( 1u, td1_->logs_ );
  EXPECT_EQ( 1u, td2_->logs_ );
  EXPECT_EQ( 1u, td3_->logs_ );

  multi_.log("two");
  EXPECT_EQ( 2u, td1_->logs_ );
  EXPECT_EQ( 2u, td2_->logs_ );
  EXPECT_EQ( 2u, td3_->logs_ );
}


struct CopyMoveDst final: public Foregin
{
  void logImpl(Entry e) override
  {
    EXPECT_EQ( 2u, e.size() );
  }
  void reloadImpl() override { }
};

TEST_F(ButLogDestinationMultiForegin, ArgumentsAreMoveToLastDestinationOnlySmokeTest)
{
  auto cp1 = std::make_shared<CopyMoveDst>();
  auto cp2 = std::make_shared<CopyMoveDst>();
  auto mv  = std::make_shared<CopyMoveDst>();
  MultiForegin multi{ {cp1, cp2, mv} };
  multi.log( "answer: ", 42 );
}


TEST_F(ButLogDestinationMultiForegin, ExceptionInAnyPrinterDoesNotStopProcessing)
{
  td1_->throws_ = true;
  td2_->throws_ = true;
  td3_->throws_ = true;

  multi_.log("one");
  EXPECT_EQ( 1u, td1_->logs_ );
  EXPECT_EQ( 1u, td2_->logs_ );
  EXPECT_EQ( 1u, td3_->logs_ );
}


TEST_F(ButLogDestinationMultiForegin, ReloadingReloadsAllDestinations)
{
  multi_.reload();
  EXPECT_EQ( 1u, td1_->reloads_ );
  EXPECT_EQ( 1u, td2_->reloads_ );
  EXPECT_EQ( 1u, td3_->reloads_ );
}


TEST_F(ButLogDestinationMultiForegin, ExceptionInAnyDestinationDoesNotStopProcessing)
{
  td1_->throws_ = true;
  td2_->throws_ = true;
  td3_->throws_ = true;

  multi_.reload();
  EXPECT_EQ( 1u, td1_->reloads_ );
  EXPECT_EQ( 1u, td2_->reloads_ );
  EXPECT_EQ( 1u, td3_->reloads_ );
}

}
