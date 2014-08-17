#include <string>
#include <sstream>
#include <cassert>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "gmock/gmock.h"
#include "Dispatcher.hpp"

using But::Pattern::Dispatcher;
using But::Pattern::AutoDispatcher;

namespace
{

struct BinaryData
{
  unsigned    id_;
  std::string data_;
};


struct MsgOne
{
  static constexpr unsigned type() { return 666; }
  int a_;
  int b_;

  template<class Archive>
  void serialize(Archive & ar, unsigned int version)
  {
    ar & version;
    ar & a_;
    ar & b_;
  }
};

bool operator==(MsgOne const& lhs, MsgOne const& rhs)
{
  return lhs.a_ == rhs.a_ && lhs.b_ == rhs.b_;
}


struct MsgTwo
{
  static constexpr unsigned type() { return 42; }
  std::string key_;
  std::string value_;

  template<class Archive>
  void serialize(Archive & ar, unsigned int version)
  {
    ar & version;
    ar & key_;
    ar & value_;
  }
};

bool operator==(MsgTwo const& lhs, MsgTwo const&rhs)
{
  return lhs.key_ == rhs.key_ && lhs.value_ == rhs.value_;
}


struct TestPolicy
{
  using IdType       = unsigned;
  using BinaryFormat = BinaryData;

  template<typename M>
  static constexpr IdType getId() { return M::type(); }
  static IdType getId(BinaryFormat const& bin) { return bin.id_; }

  template<typename M>
  static M deserialize(BinaryFormat const& bin)
  {
    assert( bin.id_ == M::type() && "invalid deserializer called" );
    std::stringstream             os(bin.data_);
    boost::archive::text_iarchive ia(os);
    M m;
    ia >> m;
    return m;
  }
};


template<typename M>
static TestPolicy::BinaryFormat serialize(M const& m)
{
  std::stringstream             os;
  boost::archive::text_oarchive oa(os);
  oa << m;
  return { M::type(), os.str() };
}

using TestDispatherBase = Dispatcher<TestPolicy::BinaryFormat>;

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
