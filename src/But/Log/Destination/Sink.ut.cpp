#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <But/Log/Destination/Sink.hpp>
#include <But/Log/Destination/SinkMock.utg.hpp>

using testing::_;
using But::Log::Destination::Sink;


namespace
{

struct ButLogDestinationSink: public testing::Test
{
  testing::StrictMock<But::Log::Destination::SinkMock> mock_;
};


TEST_F(ButLogDestinationSink, NothingIsCalledByDefault)
{
  // no calls - no logs
}

TEST_F(ButLogDestinationSink, ReloadForwardingWorks)
{
  EXPECT_CALL( mock_, reloadImpl() )
    .Times(1);
  mock_.reload();
}


TEST_F(ButLogDestinationSink, LoggingIsForwarded)
{
  EXPECT_CALL( mock_, logImpl(std::string{"narf"}) )
    .Times(1);
  mock_.log("narf");
}


TEST_F(ButLogDestinationSink, FlusingIsForwarded)
{
  EXPECT_CALL( mock_, flushImpl() )
    .Times(1);
  mock_.flush();
}

}
