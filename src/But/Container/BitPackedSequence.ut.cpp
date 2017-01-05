#include <random>
#include <cassert>
#include "gtest/gtest.h"
#include "BitPackedSequence.hpp"

using But::Container::BitPackedSequence;

namespace
{

enum class Elem
{
  X = 0,
  Y = 1,
  Z = 2
};

std::ostream& operator<<(std::ostream& os, const Elem e)
{
  switch(e)
  {
    case Elem::X: return os << "Elem::X";
    case Elem::Y: return os << "Elem::Y";
    case Elem::Z: return os << "Elem::Z";
  }
  assert(!"unknown value");
  return os << "Elem::?";
}


struct Packer
{
  static constexpr unsigned bits_count = 2;

  static auto encode(const Elem e)
  {
    return static_cast<uint8_t>(e);
  }

  static auto decode(uint8_t v)
  {
    assert( v <= 2u && "element is over Elem::Z" );
    return static_cast<Elem>(v);
  }
};


struct ButContainerBitPackedSequence: public testing::Test
{
  using Data = BitPackedSequence<Elem, Packer>;
  Data        d_;
  Data const& cd_ = d_;
};


TEST_F(ButContainerBitPackedSequence, UsualTypedefsArePresentAndValid)
{
  EXPECT_TRUE( ( std::is_same<Data::size_type, std::vector<Elem>::size_type>::value ) );
  EXPECT_TRUE( ( std::is_same<Data::value_type, Elem>::value) );
}


TEST_F(ButContainerBitPackedSequence, EmptyContainerIsFine)
{
  EXPECT_TRUE( cd_.empty() );
  EXPECT_EQ( cd_.size(), 0u );
  EXPECT_TRUE( begin(d_)  == end(d_) );
  EXPECT_TRUE( begin(cd_) == end(cd_) );
  EXPECT_TRUE( cbegin(d_) == cend(d_) );
}


TEST_F(ButContainerBitPackedSequence, AddingElementsToContainersFirstByte)
{
  d_.push_back(Elem::X);
  d_.push_back(Elem::Y);
  d_.push_back(Elem::Z);
  EXPECT_EQ( cd_.size(), 3u );
  EXPECT_EQ( cd_[0], Elem::X );
  EXPECT_EQ( cd_[1], Elem::Y );
  EXPECT_EQ( cd_[2], Elem::Z );
}


TEST_F(ButContainerBitPackedSequence, AddingElementsToContainersNextBytes)
{
  d_.push_back(Elem::Z);
  d_.push_back(Elem::Y);
  d_.push_back(Elem::X);
  d_.push_back(Elem::Z);
  d_.push_back(Elem::X);
  d_.push_back(Elem::Y);
  d_.push_back(Elem::Z);
  EXPECT_EQ( cd_.size(), 7u );
  EXPECT_EQ( cd_[0], Elem::Z );
  EXPECT_EQ( cd_[1], Elem::Y );
  EXPECT_EQ( cd_[2], Elem::X );
  EXPECT_EQ( cd_[3], Elem::Z );
  EXPECT_EQ( cd_[4], Elem::X );
  EXPECT_EQ( cd_[5], Elem::Y );
  EXPECT_EQ( cd_[6], Elem::Z );
}


// just a happy abuse of static polymorphism... ;)
struct CountingVector: std::vector<uint8_t>
{
  CountingVector() { count_ = 0; }

  void push_back(value_type const& v)
  {
    ++count_;
    std::vector<uint8_t>::push_back(v);
  }

  static unsigned count_;
};
unsigned CountingVector::count_;


TEST_F(ButContainerBitPackedSequence, ElementsAreBitPacked)
{
  // TODO: this test does not work correctly...
#if 1
  FAIL() << "fix me plz...";
  return;
#else
  using Data = BitPackedSequence<Elem, Packer, CountingVector>;
  Data d;
  EXPECT_EQ( CountingVector::count_, 0u );

  d_.push_back(Elem::Z);
  EXPECT_EQ( CountingVector::count_, 1u );

  d_.push_back(Elem::Z);
  d_.push_back(Elem::Z);
  d_.push_back(Elem::Z);
  EXPECT_EQ( CountingVector::count_, 1u );

  d_.push_back(Elem::Z);
  EXPECT_EQ( CountingVector::count_, 2u );

  d_.push_back(Elem::Z);
  d_.push_back(Elem::Z);
  d_.push_back(Elem::Z);
  EXPECT_EQ( CountingVector::count_, 2u );
#endif
}


TEST_F(ButContainerBitPackedSequence, RemovingElements)
{
  // TODO
}


TEST_F(ButContainerBitPackedSequence, ReplacingElements)
{
  // TODO
}


TEST_F(ButContainerBitPackedSequence, RandomAccess)
{
  // TODO
}


TEST_F(ButContainerBitPackedSequence, Iterating)
{
  // TODO
}


TEST_F(ButContainerBitPackedSequence, ConstIterating)
{
  // TODO
}


TEST_F(ButContainerBitPackedSequence, ReverseIterating)
{
  // TODO
}


TEST_F(ButContainerBitPackedSequence, ConstReverseIterating)
{
  // TODO
}


enum class OddElem
{
  X =  1 | 1,
  Y =  2 | 1,
  Z =  4 | 1,
  W =  8 | 1,
  Q = 16 | 1
};


struct OddPacker
{
  static constexpr unsigned bits_count = 5;

  static auto encode(const OddElem e)
  {
    const auto tmp = static_cast<uint8_t>(e);
    assert( tmp < (1<<bits_count) );
    return tmp;
  }

  static auto decode(uint8_t v)
  {
    assert( v <= (16u|1u) && "element is over OddElem::Q" );
    return static_cast<OddElem>(v);
  }
};


TEST_F(ButContainerBitPackedSequence, OddPackerSanityTests)
{
  EXPECT_EQ( OddPacker::decode( OddPacker::encode(OddElem::X) ), OddElem::X );
  EXPECT_EQ( OddPacker::decode( OddPacker::encode(OddElem::Y) ), OddElem::Y );
  EXPECT_EQ( OddPacker::decode( OddPacker::encode(OddElem::Z) ), OddElem::Z );
  EXPECT_EQ( OddPacker::decode( OddPacker::encode(OddElem::W) ), OddElem::W );
  EXPECT_EQ( OddPacker::decode( OddPacker::encode(OddElem::Q) ), OddElem::Q );
}


TEST_F(ButContainerBitPackedSequence, WorksWithOddBitCount)
{
  using Data = BitPackedSequence<OddElem, OddPacker>;
  Data d;
  d.push_back(OddElem::X);
  ASSERT_EQ( d.size(), 1u );
  EXPECT_EQ( d[0], OddElem::X );
}


TEST_F(ButContainerBitPackedSequence, WorksWithOddBitCountOnTwoBytes)
{
  using Data = BitPackedSequence<OddElem, OddPacker>;
  Data d;
  d.push_back(OddElem::X);
  d.push_back(OddElem::Z);
  ASSERT_EQ( d.size(), 2u );
  EXPECT_EQ( d[0], OddElem::X );
  EXPECT_EQ( d[1], OddElem::Z );
}


TEST_F(ButContainerBitPackedSequence, OddBitsCountSpanningMultipleElementsWorks)
{
  using Data = BitPackedSequence<OddElem, OddPacker>;
  Data d;
  d.push_back(OddElem::X);
  d.push_back(OddElem::Y);
  d.push_back(OddElem::Z);
  d.push_back(OddElem::W);
  d.push_back(OddElem::Q);
  d.push_back(OddElem::Y);
  d.push_back(OddElem::Z);
  d.push_back(OddElem::W);
  ASSERT_EQ( d.size(), 8u );
  EXPECT_EQ( d[0], OddElem::X );
  EXPECT_EQ( d[1], OddElem::Y );
  EXPECT_EQ( d[2], OddElem::Z );
  EXPECT_EQ( d[3], OddElem::W );
  EXPECT_EQ( d[4], OddElem::Q );
  EXPECT_EQ( d[5], OddElem::Y );
  EXPECT_EQ( d[6], OddElem::Z );
  EXPECT_EQ( d[7], OddElem::W );
}


auto generateLongSequence(unsigned count)
{
  std::vector<OddElem> out;
  out.reserve(count);
  const OddElem input[] = { OddElem::X, OddElem::Y, OddElem::Z, OddElem::W, OddElem::Q, OddElem::Y, OddElem::Z, OddElem::W };
  std::mt19937 gen{count};
  std::uniform_int_distribution<unsigned> dist{0, sizeof(input)-1};
  for(auto i=0u; i<count; ++i)
    out.push_back( input[ dist(gen) ] );
  return out;
}

TEST_F(ButContainerBitPackedSequence, OddBitsSmokeTestOnHugeSequence)
{
  const auto ref = generateLongSequence(1666);
  using Data = BitPackedSequence<OddElem, OddPacker>;
  Data d;
  for(auto e: ref)
    d.push_back(e);
  ASSERT_EQ( d.size(), ref.size() );
  for(auto i=0u; i<d.size(); ++i)
    EXPECT_EQ( d[i], ref[i] ) << "at position " << i;
}


enum class MaxElem
{
  X = 0u,
  Y = 0xFFu,
};


struct MaxPacker
{
  static constexpr unsigned bits_count = 8;

  static auto encode(const MaxElem e)
  {
    return static_cast<uint8_t>(e);
  }

  static auto decode(uint8_t v)
  {
    return static_cast<MaxElem>(v);
  }
};


TEST_F(ButContainerBitPackedSequence, WorksWithUpTo8Bits)
{
  using Data = BitPackedSequence<MaxElem, MaxPacker>;
  Data d;
  // TODO
}


TEST_F(ButContainerBitPackedSequence, WorksWithDequeAsUnderlyingContainer)
{
  // TODO
}


TEST_F(ButContainerBitPackedSequence, WorksWithArrayAsUnderlyingContainer)
{
  // TODO
}

}
