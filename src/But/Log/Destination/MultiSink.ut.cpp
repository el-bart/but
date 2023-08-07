#include <But/Log/Destination/MultiSink.hpp>
#include <gtest/gtest.h>

using But::makeSharedNN;
using But::NotNullShared;
using But::Log::Destination::Sink;
using But::Log::Destination::MultiSink;

namespace
{

template<int N>
struct TestDst final: public Sink
{
  void logImpl(std::string&&) override
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


struct ButLogDestinationMultiSink: public testing::Test
{
  NotNullShared<TestDst<1>> td1_{ makeSharedNN<TestDst<1>>() };
  NotNullShared<TestDst<2>> td2_{ makeSharedNN<TestDst<2>>() };
  NotNullShared<TestDst<3>> td3_{ makeSharedNN<TestDst<3>>() };
  MultiSink multi_{ {td1_, td2_, td3_} };
};


TEST_F(ButLogDestinationMultiSink, NoDestinationsSmokeTest)
{
  MultiSink m{ {} };
  m.log("test");
}


TEST_F(ButLogDestinationMultiSink, SingleDestinationWorks)
{
  MultiSink m{ {td1_} };
  m.log("test");
  EXPECT_EQ( 1u, td1_->logs_ );
}


TEST_F(ButLogDestinationMultiSink, PrintingGoesThroughAllDestinations)
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


TEST_F(ButLogDestinationMultiSink, ExceptionInAnyPrinterDoesNotStopProcessing)
{
  td1_->throws_ = true;
  td2_->throws_ = true;
  td3_->throws_ = true;

  multi_.log("one");
  EXPECT_EQ( 1u, td1_->logs_ );
  EXPECT_EQ( 1u, td2_->logs_ );
  EXPECT_EQ( 1u, td3_->logs_ );
}


TEST_F(ButLogDestinationMultiSink, ReloadingReloadsAllDestinations)
{
  multi_.reload();
  EXPECT_EQ( 1u, td1_->reloads_ );
  EXPECT_EQ( 1u, td2_->reloads_ );
  EXPECT_EQ( 1u, td3_->reloads_ );
}


TEST_F(ButLogDestinationMultiSink, ExceptionInAnyReloadDoesNotStopProcessing)
{
  td1_->throws_ = true;
  td2_->throws_ = true;
  td3_->throws_ = true;

  multi_.reload();
  EXPECT_EQ( 1u, td1_->reloads_ );
  EXPECT_EQ( 1u, td2_->reloads_ );
  EXPECT_EQ( 1u, td3_->reloads_ );
}


TEST_F(ButLogDestinationMultiSink, FlusingFlushesAllDestinations)
{
  multi_.flush();
  EXPECT_EQ( 1u, td1_->flushes_ );
  EXPECT_EQ( 1u, td2_->flushes_ );
  EXPECT_EQ( 1u, td3_->flushes_ );
}


TEST_F(ButLogDestinationMultiSink, ExceptionInAnyFlushDoesNotStopProcessing)
{
  td1_->throws_ = true;
  td2_->throws_ = true;
  td3_->throws_ = true;

  multi_.flush();
  EXPECT_EQ( 1u, td1_->flushes_ );
  EXPECT_EQ( 1u, td2_->flushes_ );
  EXPECT_EQ( 1u, td3_->flushes_ );
}


TEST_F(ButLogDestinationMultiSink, ConstructingFromExplicitListOfPointersToDerivedClasses)
{
  MultiSink m{td1_, td2_, td3_};

  m.log("first");
  EXPECT_EQ( 1u, td1_->logs_ );
  EXPECT_EQ( 1u, td2_->logs_ );
  EXPECT_EQ( 1u, td3_->logs_ );

  m.log("second");
  EXPECT_EQ( 2u, td1_->logs_ );
  EXPECT_EQ( 2u, td2_->logs_ );
  EXPECT_EQ( 2u, td3_->logs_ );

  multi_.flush();
  EXPECT_EQ( 1u, td1_->flushes_ );
  EXPECT_EQ( 1u, td2_->flushes_ );
  EXPECT_EQ( 1u, td3_->flushes_ );

  multi_.reload();
  EXPECT_EQ( 1u, td1_->reloads_  );
  EXPECT_EQ( 1u, td2_->reloads_  );
  EXPECT_EQ( 1u, td3_->reloads_  );
}

}
