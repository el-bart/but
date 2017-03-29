#include "gtest/gtest.h"
#include "Filter.hpp"
#include "ForeignMock.ut.hpp"

using testing::_;
using testing::StrictMock;
using But::Log::Backend::Entry;
using But::Log::Destination::Filter;
using But::Log::Destination::Foreign;
using But::Log::Destination::ForeignMock;

namespace
{

struct ButLogDestinationFilter: public testing::Test
{
  But::NotNull<std::shared_ptr<StrictMock<ForeignMock>>> mock_{ std::make_shared<StrictMock<ForeignMock>>() };
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
