#include <gmock/gmock.h>
#include <But/Pattern/Dispatcher.hpp>
#include <But/Pattern/DispatcherTestStructure.utg.hpp>

using But::Pattern::Dispatcher;
using But::Pattern::AutoDispatcher;

namespace
{

struct TestDispatcher final: public AutoDispatcher<TestDispatcher, TestPolicy,
                                                   MsgOne,
                                                   MsgTwo>
{
  MOCK_METHOD1(handle, void(MsgOne const&));
  MOCK_METHOD1(handle, void(MsgTwo const&));
};

struct ButPatternDispatcher: public testing::Test
{
  TestDispatcher td_;
};


TEST_F(ButPatternDispatcher, MsgOneSanityCheck)
{
  using MsgType = MsgOne;
  const auto bin = serialize( MsgType{42, 69} );
  EXPECT_EQ( MsgType::type(), bin.id_ );
  const auto msg = TestPolicy::deserialize<MsgType>(bin);
  EXPECT_EQ( 42, msg.a_ );
  EXPECT_EQ( 69, msg.b_ );
}


TEST_F(ButPatternDispatcher, MsgTwoSanityCheck)
{
  using MsgType = MsgTwo;
  const auto bin = serialize( MsgType{"answer", "42"} );
  EXPECT_EQ( MsgType::type(), bin.id_ );
  const auto msg = TestPolicy::deserialize<MsgType>(bin);
  EXPECT_EQ( "answer", msg.key_ );
  EXPECT_EQ( "42",     msg.value_ );
}


struct EmptyTestDispatcher final: public AutoDispatcher<EmptyTestDispatcher, TestPolicy>
{ };

TEST_F(ButPatternDispatcher, EmptyDispatcher)
{
  EmptyTestDispatcher etd;      // should compile
  EXPECT_THROW( etd.dispatch({}), EmptyTestDispatcher::UnknownMessage );
}


TEST_F(ButPatternDispatcher, InvalidId)
{
  EXPECT_THROW( td_.dispatch( {1111, "oops"} ), EmptyTestDispatcher::UnknownMessage );
}


TEST_F(ButPatternDispatcher, DispatchOneMessage)
{
  const MsgOne m{1,2};
  EXPECT_CALL(td_, handle(m));
  td_.dispatch( serialize(m) );
}


TEST_F(ButPatternDispatcher, DispatchTwoMessages)
{
  const MsgOne m1{1, 2};
  const MsgTwo m2{"foo", "bar"};
  EXPECT_CALL(td_, handle(m1));
  EXPECT_CALL(td_, handle(m2));
  td_.dispatch( serialize(m2) );
  td_.dispatch( serialize(m1) );
}

}
