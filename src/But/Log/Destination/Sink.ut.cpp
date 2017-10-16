#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <But/Log/Destination/Sink.hpp>
#include <But/Log/Destination/Common/rootElementTag.hpp>
#include <But/Log/Destination/SinkMock.ut.hpp>

using testing::_;
using But::Log::Destination::Sink;
using But::Log::Destination::Common::rootElementTag;
using But::Log::Backend::Tag;
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
  const auto expected = FieldInfo{ rootElementTag(), std::move(data) };
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
  const auto expected = FieldInfo{ rootElementTag(), std::move(data) };
  EXPECT_CALL( mock_, logImpl(expected) )
    .Times(1);
  mock_.log(fmt, 42, "foo bar", 3.14);
}


TEST_F(ButLogDestinationSink, ReloadForwardingWorks)
{
  EXPECT_CALL( mock_, reloadImpl() )
    .Times(1);
  mock_.reload();
}


TEST_F(ButLogDestinationSink, PassingFieldInfoTypeDirectory)
{
  std::vector<FieldInfo> data{ FieldInfo{42}, FieldInfo{"foo bar"}, };
  const auto expected = FieldInfo{ Tag{"xx"}, std::move(data) };
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
