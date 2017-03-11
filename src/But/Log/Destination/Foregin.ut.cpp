#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Foregin.hpp"

using testing::_;
using But::Log::Destination::Foregin;
using But::Log::Backend::Entry;
using But::Log::Backend::FieldInfo;

namespace
{

struct DestinationMock: public Foregin
{
  MOCK_METHOD1(logImpl, void(Entry));
  MOCK_METHOD0(reloadImpl, void());
};


struct ButLogDestinationForegin: public testing::Test
{
  testing::StrictMock<DestinationMock> mock_;
};


TEST_F(ButLogDestinationForegin, NothingIsCalledByDefault)
{
  // no calls - no logs
}


TEST_F(ButLogDestinationForegin, ForwardingWithProperTypes)
{
  const std::vector<FieldInfo> expected{
          FieldInfo{42},
          FieldInfo{"foo bar"},
          FieldInfo{3.14}
        };
  EXPECT_CALL( mock_, logImpl(expected) )
    .Times(1);
  mock_.log(42, "foo bar", 3.14);
}


TEST_F(ButLogDestinationForegin, CheckArrowOperator)
{
  EXPECT_CALL( mock_, logImpl(_) )
    .Times(1);
  mock_->log(42, "foo bar", 3.14);
}


TEST_F(ButLogDestinationForegin, ReloadForwardingWorks)
{
  EXPECT_CALL( mock_, reloadImpl() )
    .Times(1);
  mock_->reload();
}


TEST_F(ButLogDestinationForegin, PassingEntryTypeDirectory)
{
  const std::vector<FieldInfo> expected{ FieldInfo{42}, FieldInfo{"foo bar"}, };
  EXPECT_CALL( mock_, logImpl(expected) )
    .Times(1);
  mock_.log(expected);
}

}
