#include "gtest/gtest.h"
#include "Filter.hpp"
#include "SinkMock.ut.hpp"

using testing::_;
using testing::StrictMock;
using But::Log::Field::FormattedString;
using But::Log::Backend::FieldInfo;
using But::Log::Destination::Filter;
using But::Log::Destination::Sink;
using But::Log::Destination::SinkMock;

namespace
{

struct ButLogDestinationFilter: public testing::Test
{
  But::NotNull<std::shared_ptr<StrictMock<SinkMock>>> mock_{ std::make_shared<StrictMock<SinkMock>>() };
  Filter filter_{ [](FieldInfo const&) { return true; }, mock_ };
};


TEST_F(ButLogDestinationFilter, ForwardingReloadRequest)
{
  EXPECT_CALL(*mock_, reloadImpl()).Times(1);
  filter_.reload();
}


TEST_F(ButLogDestinationFilter, ForwardingFlushing)
{
  EXPECT_CALL(*mock_, flushImpl()).Times(1);
  filter_.flush();
}


TEST_F(ButLogDestinationFilter, LoggingWhenConditionIsTrue)
{
  EXPECT_CALL(*mock_, logImpl(_)).Times(2);
  filter_.log("answer is: ", 42);
  filter_.log( FormattedString{"xxx"}, "answer is: ", 42);
}


TEST_F(ButLogDestinationFilter, FormattedLoggingWhenConditionIsTrue)
{
  EXPECT_CALL(*mock_, logImpl(_)).Times(2);
  filter_.log("answer is: ", 42);
  filter_.log( FormattedString{"xxx"}, "answer is: ", 42);
}


TEST_F(ButLogDestinationFilter, NotLoggingWhenConditionIsFalse)
{
  EXPECT_CALL(*mock_, logImpl(_)).Times(0);
  EXPECT_CALL(*mock_, logImpl(_)).Times(0);
  Filter reject{ [](FieldInfo const&) { return false; }, mock_ };
  reject.log("answer is: ", 42);
  reject.log( FormattedString{"xxx"}, "answer is: ", 42);
}

}
