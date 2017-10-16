#include <gmock/gmock.h>
#include <But/Pattern/DispatcherMock.hpp>
#include <But/Pattern/DispatcherTestStructure.ut.hpp>

using But::Pattern::AutoDispatcher;
using namespace testing;

namespace
{

struct SomeDispatcher final: public AutoDispatcher<SomeDispatcher, TestPolicy,
                                                   MsgOne,
                                                   MsgTwo>
{
  void handle(MsgOne const&);
  void handle(MsgTwo const&);
};

using Mock = BUT_PATTERN_DISPATCHER_MOCK(SomeDispatcher);

struct ButPatternDispatcherMock: public testing::Test
{
  Mock mock_;
};


struct WeirdException { };

TEST_F(ButPatternDispatcherMock, ExpectThrow)
{
  BUT_PATTERN_DISPATCHER_EXPECT_CALL(mock_, MsgOne, 1, 2)
      .WillOnce( Throw(WeirdException{}) );
  EXPECT_THROW( mock_.dispatch( serialize( MsgOne{1,2} ) ), WeirdException );
}


TEST_F(ButPatternDispatcherMock, ExpectationWithCount)
{
  BUT_PATTERN_DISPATCHER_EXPECT_CALL(mock_, MsgOne, 10, 20)
      .Times(2);
  mock_.dispatch( serialize( MsgOne{10,20} ) );
  mock_.dispatch( serialize( MsgOne{10,20} ) );
}

}
