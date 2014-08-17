#include <string>
#include <sstream>
#include <cassert>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "gmock/gmock.h"
#include "Dispatcher.hpp"

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

inline bool operator==(MsgOne const& lhs, MsgOne const& rhs)
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

inline bool operator==(MsgTwo const& lhs, MsgTwo const&rhs)
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


using TestDispatherBase = But::Pattern::Dispatcher<TestPolicy::BinaryFormat>;

}
