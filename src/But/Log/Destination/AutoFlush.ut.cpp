#include <gtest/gtest.h>
#include <But/Log/Destination/AutoFlush.hpp>
#include <But/Log/Destination/SinkMock.ut.hpp>

using testing::_;
using testing::Throw;
using But::Log::Backend::FieldInfo;
using But::Log::Destination::SinkMock;
using But::Log::Destination::AutoFlush;

namespace
{

struct ButLogDestinationAutoFlush: public testing::Test
{
  But::NotNullShared<SinkMock> mock_{ But::makeSharedNN<SinkMock>() };
  AutoFlush sink_{mock_};
};


TEST_F(ButLogDestinationAutoFlush, LoggingCausesFlushing)
{
  testing::Sequence seq;
  EXPECT_CALL((*mock_), logImpl(_) ).Times(1).InSequence(seq);
  EXPECT_CALL((*mock_), flushImpl() ).Times(1).InSequence(seq);
  sink_.log("foo-bar");
}


TEST_F(ButLogDestinationAutoFlush, FlushIsForwarded)
{
  EXPECT_CALL((*mock_), flushImpl() );
  sink_.flush();
}


TEST_F(ButLogDestinationAutoFlush, ReloadIsForwarded)
{
  EXPECT_CALL((*mock_), reloadImpl() );
  sink_.reload();
}


TEST_F(ButLogDestinationAutoFlush, ExceptionFromLogGetsForwarded)
{
  struct Dummy { };
  EXPECT_CALL((*mock_), logImpl(_) ).WillOnce( testing::Throw(Dummy{}) );
  EXPECT_THROW( sink_.log("kaboom!"), Dummy );
}


TEST_F(ButLogDestinationAutoFlush, ExceptionFromFlushGetsForwarded)
{
  struct Dummy { };
  EXPECT_CALL((*mock_), flushImpl() ).WillOnce( testing::Throw(Dummy{}) );
  EXPECT_THROW( sink_.flush(), Dummy );
}


TEST_F(ButLogDestinationAutoFlush, ExceptionFromReloadGetsForwarded)
{
  struct Dummy { };
  EXPECT_CALL((*mock_), reloadImpl() ).WillOnce( testing::Throw(Dummy{}) );
  EXPECT_THROW( sink_.reload(), Dummy );
}

}
