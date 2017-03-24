#include "gtest/gtest.h"
#include "Filter.hpp"
#include "ForeginMock.ut.hpp"

using testing::_;
using testing::StrictMock;
using But::Log::Destination::Filter;
using But::Log::Destination::Foregin;
using But::Log::Destination::ForeginMock;

namespace
{

struct ButLogDestinationFilter: public testing::Test
{
  But::NotNull<std::shared_ptr<StrictMock<ForeginMock>>> mock_{ std::make_shared<StrictMock<ForeginMock>>() };
  Filter filter_{ [](Entry const&) { return true; }, mock_ };
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
  EXPECT_CALL(*mock_, logImpl(_)).Times(1);
  filter_.log("answer is: ", 42);
}


TEST_F(ButLogDestinationFilter, NotLoggingWhenConditionIsFalse)
{
  EXPECT_CALL(*mock_, logImpl(_)).Times(0);
  Filter reject{ [](Entry const&) { return false; }, mock_ };
  reject.log("answer is: ", 42);
}

}
