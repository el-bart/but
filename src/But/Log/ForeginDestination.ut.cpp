#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ForeginDestination.hpp"

using But::Log::ForeginDestination;
using But::Log::Backend::Entry;
using But::Log::Backend::FieldInfo;

namespace
{

struct DestinationMock: public ForeginDestination
{
  MOCK_METHOD1(logImpl, void(Entry));
};


struct ButLogForeginDestination: public testing::Test
{
  testing::StrictMock<DestinationMock> mock_;
};


TEST_F(ButLogForeginDestination, NothingIsCalledByDefault)
{
  // no calls - no logs
}


TEST_F(ButLogForeginDestination, ForwardingWithProperTypes)
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

}
