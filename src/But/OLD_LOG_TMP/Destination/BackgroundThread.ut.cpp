#include <chrono>
#include <thread>
#include <atomic>
#include <gtest/gtest.h>
#include <But/Log/Destination/BackgroundThread.hpp>
#include <But/Log/Destination/SinkMock.ut.hpp>
#include <But/Threading/Event.hpp>
#include <But/Threading/JoiningThread.hpp>
#include <But/Log/Destination/detail/args2FieldInfo.hpp>

using testing::_;
using testing::Throw;
using But::Log::Backend::FieldInfo;
using But::Log::Destination::Sink;
using But::Log::Destination::SinkMock;
using But::Log::Destination::BackgroundThread;
using But::Log::Destination::detail::args2FieldInfo;
using Thread = But::Threading::JoiningThread<std::thread>;

namespace
{

struct ButLogDestinationBackgroundThread: public testing::Test
{
  But::NotNullShared<SinkMock> mock_{ But::makeSharedNN<SinkMock>() };
  BackgroundThread sink_{mock_, 1000*1000};
};


TEST_F(ButLogDestinationBackgroundThread, LoggingWorks)
{
  EXPECT_CALL((*mock_), logImpl(_) );
  sink_.log( args2FieldInfo("whatever") );
}


struct IdLoggingSink: public Sink
{
  void logImpl(FieldInfo const&) override { logThreadId_  = std::this_thread::get_id(); }
  void flushImpl() override { }
  void reloadImpl() override { }

  std::thread::id logThreadId_;
};

TEST_F(ButLogDestinationBackgroundThread, LoggingIsDoneInSeparateThrad)
{
  auto idLogSink = But::makeSharedNN<IdLoggingSink>();
  {
    BackgroundThread sink{idLogSink, 1000*1000};
    sink.log( args2FieldInfo("whatever") );
  }
  EXPECT_NE( std::thread::id{}, idLogSink->logThreadId_ );
  EXPECT_NE( std::this_thread::get_id(), idLogSink->logThreadId_ );
}


TEST_F(ButLogDestinationBackgroundThread, ReloadingGetsForwarded)
{
  EXPECT_CALL((*mock_), reloadImpl() );
  sink_.reload();
}


TEST_F(ButLogDestinationBackgroundThread, FLushingGetsForwarded)
{
  EXPECT_CALL((*mock_), flushImpl() );
  sink_.flush();
}


TEST_F(ButLogDestinationBackgroundThread, AllLogsAreFlushedBeforeStoppingBackgroundThread)
{
  constexpr auto count = 100;
  EXPECT_CALL((*mock_), logImpl(_) ).Times( testing::Exactly(count) );
  for(auto i=0; i<count; ++i)
    sink_.log( args2FieldInfo("x") );
}


TEST_F(ButLogDestinationBackgroundThread, FlushingBLocksUntilAllLogsAreLoggedAndFlushFinishes)
{
  constexpr auto count = 100;
  testing::Sequence seq;
  EXPECT_CALL((*mock_), logImpl(_) ).Times( testing::AtMost(1) );   // if previously last log was done in parallel
  EXPECT_CALL((*mock_), logImpl(_) )
    .Times( testing::Between(count-1, count) )  // last log might actually happen to be executed in parallel to its being logged
    .InSequence(seq)
    .RetiresOnSaturation();
  EXPECT_CALL((*mock_), flushImpl() )
    .InSequence(seq);
  for(auto i=0; i<count; ++i)
    sink_.log( args2FieldInfo("x") );
  sink_.flush();
}


TEST_F(ButLogDestinationBackgroundThread, ExceptionsFromChainedSinkLogsAreIgnored)
{
  EXPECT_CALL((*mock_), logImpl(_) ).WillRepeatedly( Throw( std::runtime_error{"should be ignored"} ) );
  EXPECT_NO_THROW( sink_.log( args2FieldInfo("error") ) );
}


TEST_F(ButLogDestinationBackgroundThread, ExceptionsFromChainedFlushAndReloadAreForwarded)
{
  struct TestError { };
  EXPECT_CALL((*mock_), reloadImpl() ).WillRepeatedly( Throw( TestError{} ) );
  EXPECT_CALL((*mock_), flushImpl() ).WillRepeatedly( Throw( TestError{} ) );

  EXPECT_THROW( sink_.reload(), TestError );
  EXPECT_THROW( sink_.flush(), TestError );
}


TEST_F(ButLogDestinationBackgroundThread, QueuesSizeIsObeyed)
{
  using Clock = std::chrono::high_resolution_clock;
  Clock::time_point loggedAt;
  Clock::time_point unblockedAt;

  EXPECT_CALL( (*mock_), logImpl(_) ).Times( testing::AtLeast(1) );

  But::Threading::Event e;
  auto block = [&](FieldInfo const&) { EXPECT_TRUE( e.wait() ); };
  EXPECT_CALL( (*mock_), logImpl(_) )
    .WillOnce( testing::Invoke(block) )
    .RetiresOnSaturation();

  BackgroundThread sink{mock_, 2};
  sink.log( args2FieldInfo("x") );
  sink.log( args2FieldInfo("x") );
  sink.log( args2FieldInfo("x") );

  {
    Thread th1{ [&] {
      sink.log( args2FieldInfo("x") );
      loggedAt = Clock::now();
    } };
    Thread th2{ [&] {
      unblockedAt = Clock::now();
      e.set();
    } };
  }

  EXPECT_LE( unblockedAt, loggedAt );
}


TEST_F(ButLogDestinationBackgroundThread, SizeOfZeroMeansNoLimit)
{
  EXPECT_CALL( (*mock_), logImpl(_) ).Times( testing::AtLeast(1) );
  BackgroundThread sink{mock_, 0};
  for(auto i=0; i<100; ++i)
    sink.log( args2FieldInfo("x") );
}

}
