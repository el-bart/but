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


TEST_F(ButContainerBitPackedSequence, AddingElementsToContainer)
{
  d_.push_back(Elem::Z);
  d_.push_back(Elem::Y);
  d_.push_back(Elem::X);
  EXPECT_EQ( cd_.size(), 3u );
  EXPECT_EQ( cd_[0], Elem::Z );
  EXPECT_EQ( cd_[1], Elem::Y );
  EXPECT_EQ( cd_[2], Elem::X );
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
  X = 0,
  Y = 1,
  Z = 2,
  W = 3,
  Q = 4
};


struct OddPacker
{
  static constexpr unsigned bits_count = 3;

  static auto encode(const OddElem e)
  {
    const auto tmp = static_cast<uint8_t>(e);
    assert( tmp < (1<<(bits_count-1)) );
    return tmp;
  }

  static auto decode(uint8_t v)
  {
    assert( v <= 3u && "element is over OddElem::Q" );
    return static_cast<OddElem>(v);
  }
};


TEST_F(ButContainerBitPackedSequence, WorksWithOddBitCount)
{
  using Data = BitPackedSequence<OddElem, OddPacker>;
  Data d;
  // TODO
}


enum class MaxElem
{
  X = 0u,
  Y = 255u,
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
