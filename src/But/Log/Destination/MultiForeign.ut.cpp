#include "gtest/gtest.h"
#include "MultiForeign.hpp"

using But::makeSharedNN;
using But::NotNullShared;
using But::Log::Backend::Entry;
using But::Log::Destination::Foreign;
using But::Log::Destination::MultiForeign;

namespace
{

template<int N>
struct TestDst final: public Foreign
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

  void flushImpl() override
  {
    ++flushes_;
    if(throws_)
      throw std::runtime_error{"throwing, as requested"};
  }

  bool throws_{false};
  unsigned logs_{0};
  unsigned reloads_{0};
  unsigned flushes_{0};
};


struct ButLogDestinationMultiForeign: public testing::Test
{
  NotNullShared<TestDst<1>> td1_{ makeSharedNN<TestDst<1>>() };
  NotNullShared<TestDst<2>> td2_{ makeSharedNN<TestDst<2>>() };
  NotNullShared<TestDst<3>> td3_{ makeSharedNN<TestDst<3>>() };
  MultiForeign multi_{ {td1_, td2_, td3_} };
};


TEST_F(ButLogDestinationMultiForeign, NoDestinationsSmokeTest)
{
  MultiForeign m{ {} };
  m.log("test");
}


TEST_F(ButLogDestinationMultiForeign, SingleDestinationWorks)
{
  MultiForeign m{ {td1_} };
  m.log("test");
  EXPECT_EQ( 1u, td1_->logs_ );
}


TEST_F(ButLogDestinationMultiForeign, PrintingGoesThroughAllDestinations)
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


struct CopyMoveDst final: public Foreign
{
  void logImpl(Entry e) override
  {
    EXPECT_EQ( 2u, e.size() );
  }
  void reloadImpl() override { }
  void flushImpl() override { }
};

TEST_F(ButLogDestinationMultiForeign, ArgumentsAreMoveToLastDestinationOnlySmokeTest)
{
  auto cp1 = makeSharedNN<CopyMoveDst>();
  auto cp2 = makeSharedNN<CopyMoveDst>();
  auto mv  = makeSharedNN<CopyMoveDst>();
  MultiForeign multi{ {cp1, cp2, mv} };
  multi.log( "answer: ", 42 );
}


TEST_F(ButLogDestinationMultiForeign, ExceptionInAnyPrinterDoesNotStopProcessing)
{
  td1_->throws_ = true;
  td2_->throws_ = true;
  td3_->throws_ = true;

  multi_.log("one");
  EXPECT_EQ( 1u, td1_->logs_ );
  EXPECT_EQ( 1u, td2_->logs_ );
  EXPECT_EQ( 1u, td3_->logs_ );
}


TEST_F(ButLogDestinationMultiForeign, ReloadingReloadsAllDestinations)
{
  multi_.reload();
  EXPECT_EQ( 1u, td1_->reloads_ );
  EXPECT_EQ( 1u, td2_->reloads_ );
  EXPECT_EQ( 1u, td3_->reloads_ );
}


TEST_F(ButLogDestinationMultiForeign, ExceptionInAnyReloadDoesNotStopProcessing)
{
  td1_->throws_ = true;
  td2_->throws_ = true;
  td3_->throws_ = true;

  multi_.reload();
  EXPECT_EQ( 1u, td1_->reloads_ );
  EXPECT_EQ( 1u, td2_->reloads_ );
  EXPECT_EQ( 1u, td3_->reloads_ );
}


TEST_F(ButLogDestinationMultiForeign, FlusingFlushesAllDestinations)
{
  multi_.flush();
  EXPECT_EQ( 1u, td1_->flushes_ );
  EXPECT_EQ( 1u, td2_->flushes_ );
  EXPECT_EQ( 1u, td3_->flushes_ );
}


TEST_F(ButLogDestinationMultiForeign, ExceptionInAnyFlushDoesNotStopProcessing)
{
  td1_->throws_ = true;
  td2_->throws_ = true;
  td3_->throws_ = true;

  multi_.flush();
  EXPECT_EQ( 1u, td1_->flushes_ );
  EXPECT_EQ( 1u, td2_->flushes_ );
  EXPECT_EQ( 1u, td3_->flushes_ );
}

}