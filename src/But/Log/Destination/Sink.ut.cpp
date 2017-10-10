#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Sink.hpp"
#include "SinkMock.ut.hpp"

using testing::_;
using But::Log::Destination::Sink;
using But::Log::Backend::Type;
using But::Log::Backend::FieldInfo;
using But::Log::Field::FormattedString;


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


TEST_F(ButLogDestinationSink, ForwardingWithProperTypes)
{
  std::vector<FieldInfo> data{
          FieldInfo{42},
          FieldInfo{"foo bar"},
          FieldInfo{3.14}
        };
  const auto expected = FieldInfo{ Type{"log"}, std::move(data) };
  EXPECT_CALL( mock_, logImpl(expected) )
    .Times(1);
  mock_.log(42, "foo bar", 3.14);
}


TEST_F(ButLogDestinationSink, ForwardingFormattedStringWithProperTypes)
{
  const auto fmt = FormattedString{"fmt-test"};
  std::vector<FieldInfo> data{
          toFieldInfo(fmt),
          FieldInfo{42},
          FieldInfo{"foo bar"},
          FieldInfo{3.14}
        };
  const auto expected = FieldInfo{ Type{"log"}, std::move(data) };
  EXPECT_CALL( mock_, logImpl(expected) )
    .Times(1);
  mock_.log(fmt, 42, "foo bar", 3.14);
}


TEST_F(ButLogDestinationSink, CheckArrowOperator)
{
  EXPECT_CALL( mock_, logImpl(_) )
    .Times(1);
  mock_->log(42, "foo bar", 3.14);
}


TEST_F(ButLogDestinationSink, ReloadForwardingWorks)
{
  EXPECT_CALL( mock_, reloadImpl() )
    .Times(1);
  mock_->reload();
}


TEST_F(ButLogDestinationSink, PassingFieldInfoTypeDirectory)
{
  std::vector<FieldInfo> data{ FieldInfo{42}, FieldInfo{"foo bar"}, };
  const auto expected = FieldInfo{ Type{"xx"}, std::move(data) };
  EXPECT_CALL( mock_, logImpl(expected) )
    .Times(1);
  mock_.log(expected);
}


TEST_F(ButLogDestinationSink, FlusingIsForwarded)
{
  EXPECT_CALL( mock_, flushImpl() )
    .Times(1);
  mock_.flush();
}

}
