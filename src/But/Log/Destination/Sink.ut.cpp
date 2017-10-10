#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Foreign.hpp"
#include "ForeignMock.ut.hpp"

using testing::_;
using But::Log::Destination::Foreign;
using But::Log::Backend::Type;
using But::Log::Backend::FieldInfo;
using But::Log::Field::FormattedString;


namespace
{

struct ButLogDestinationForeign: public testing::Test
{
  testing::StrictMock<But::Log::Destination::ForeignMock> mock_;
};


TEST_F(ButLogDestinationForeign, NothingIsCalledByDefault)
{
  // no calls - no logs
}


TEST_F(ButLogDestinationForeign, ForwardingWithProperTypes)
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


TEST_F(ButLogDestinationForeign, ForwardingFormattedStringWithProperTypes)
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


TEST_F(ButLogDestinationForeign, CheckArrowOperator)
{
  EXPECT_CALL( mock_, logImpl(_) )
    .Times(1);
  mock_->log(42, "foo bar", 3.14);
}


TEST_F(ButLogDestinationForeign, ReloadForwardingWorks)
{
  EXPECT_CALL( mock_, reloadImpl() )
    .Times(1);
  mock_->reload();
}


TEST_F(ButLogDestinationForeign, PassingFieldInfoTypeDirectory)
{
  std::vector<FieldInfo> data{ FieldInfo{42}, FieldInfo{"foo bar"}, };
  const auto expected = FieldInfo{ Type{"xx"}, std::move(data) };
  EXPECT_CALL( mock_, logImpl(expected) )
    .Times(1);
  mock_.log(expected);
}


TEST_F(ButLogDestinationForeign, FlusingIsForwarded)
{
  EXPECT_CALL( mock_, flushImpl() )
    .Times(1);
  mock_.flush();
}

}
