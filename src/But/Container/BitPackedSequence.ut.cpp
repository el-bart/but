#include <random>
#include <type_traits>
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
  template<typename T>
  auto generateLongSequence(const unsigned count, std::vector<T> const& input)
  {
    std::vector<T> out;
    out.reserve(count);
    std::mt19937 gen{count};
    std::uniform_int_distribution<typename std::vector<T>::size_type> dist{ 0, input.size()-1 };
    for(auto i=0u; i<count; ++i)
      out.push_back( input[ dist(gen) ] );
    return out;
  }

  template<typename TPacker, typename T>
  void smokeTestPackingLongSequence(std::vector<T> const& input)
  {
    const auto ref = generateLongSequence(2666, input);

    using Data = BitPackedSequence<T, TPacker>;
    Data d;
    for(auto e: ref)
      d.push_back(e);

    auto const& cd = d;
    ASSERT_EQ( d.size(), ref.size() );
    auto errors = 0u;
    for(auto i=0u; i<d.size(); ++i)
      if( cd[i] != ref[i] )
        ++errors;
    EXPECT_EQ(errors, 0u);
  }

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
  using Data = BitPackedSequence<Elem, Packer, CountingVector>;
  Data d;
  EXPECT_EQ( CountingVector::count_, 0u );

  d.push_back(Elem::Z);
  EXPECT_EQ( CountingVector::count_, 1u );

  d.push_back(Elem::Z);
  d.push_back(Elem::Z);
  d.push_back(Elem::Z);
  EXPECT_EQ( CountingVector::count_, 1u );

  d.push_back(Elem::Z);
  EXPECT_EQ( CountingVector::count_, 2u );

  d.push_back(Elem::Z);
  d.push_back(Elem::Z);
  d.push_back(Elem::Z);
  EXPECT_EQ( CountingVector::count_, 2u );
}


TEST_F(ButContainerBitPackedSequence, RemovingElements)
{
  // TODO
}


TEST_F(ButContainerBitPackedSequence, ReplacingElements)
{
  d_.push_back(Elem::X);
  d_.push_back(Elem::Y);
  d_.push_back(Elem::Z);

  d_[1] = Elem::Z;
  EXPECT_EQ( cd_.size(), 3u );
  EXPECT_EQ( cd_[0], Elem::X );
  EXPECT_EQ( cd_[1], Elem::Z );
  EXPECT_EQ( cd_[2], Elem::Z );

  d_[0] = Elem::Y;
  EXPECT_EQ( cd_.size(), 3u );
  EXPECT_EQ( cd_[0], Elem::Y );
  EXPECT_EQ( cd_[1], Elem::Z );
  EXPECT_EQ( cd_[2], Elem::Z );

  d_[2] = Elem::X;
  EXPECT_EQ( cd_.size(), 3u );
  EXPECT_EQ( cd_[0], Elem::Y );
  EXPECT_EQ( cd_[1], Elem::Z );
  EXPECT_EQ( cd_[2], Elem::X );
}


TEST_F(ButContainerBitPackedSequence, RandomAccess)
{
  d_.push_back(Elem::X);
  d_.push_back(Elem::Y);
  d_.push_back(Elem::Z);
  EXPECT_EQ( cd_.size(), 3u );
  EXPECT_EQ( cd_[0], Elem::X );
  EXPECT_EQ( cd_[1], Elem::Y );
  EXPECT_EQ( cd_[2], Elem::Z );
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


TEST_F(ButContainerBitPackedSequence, SmallBitsCountSmokeTestOnHugeSequence)
{
  const std::vector<Elem> input{ Elem::X, Elem::Y, Elem::Z };
  smokeTestPackingLongSequence<Packer>(input);
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
  auto const& cd = d;
  ASSERT_EQ( cd.size(), 1u );
  EXPECT_EQ( cd[0], OddElem::X );
}


TEST_F(ButContainerBitPackedSequence, WorksWithOddBitCountOnTwoBytes)
{
  using Data = BitPackedSequence<OddElem, OddPacker>;
  Data d;
  d.push_back(OddElem::X);
  d.push_back(OddElem::Z);
  auto const& cd = d;
  ASSERT_EQ( cd.size(), 2u );
  EXPECT_EQ( cd[0], OddElem::X );
  EXPECT_EQ( cd[1], OddElem::Z );
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
  auto const& cd = d;
  ASSERT_EQ( cd.size(), 8u );
  EXPECT_EQ( cd[0], OddElem::X );
  EXPECT_EQ( cd[1], OddElem::Y );
  EXPECT_EQ( cd[2], OddElem::Z );
  EXPECT_EQ( cd[3], OddElem::W );
  EXPECT_EQ( cd[4], OddElem::Q );
  EXPECT_EQ( cd[5], OddElem::Y );
  EXPECT_EQ( cd[6], OddElem::Z );
  EXPECT_EQ( cd[7], OddElem::W );
}


TEST_F(ButContainerBitPackedSequence, OddBitsSmokeTestOnHugeSequence)
{
  const std::vector<OddElem> input{ OddElem::X, OddElem::Y, OddElem::Z, OddElem::W, OddElem::Q, OddElem::Y, OddElem::Z, OddElem::W };
  smokeTestPackingLongSequence<OddPacker>(input);
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
  d.push_back(MaxElem::X);
  d.push_back(MaxElem::Y);
  d.push_back(MaxElem::X);
  d.push_back(MaxElem::Y);
  auto const& cd = d;
  ASSERT_EQ( cd.size(), 4u );
  EXPECT_EQ( cd[0], MaxElem::X );
  EXPECT_EQ( cd[1], MaxElem::Y );
  EXPECT_EQ( cd[2], MaxElem::X );
  EXPECT_EQ( cd[3], MaxElem::Y );
}


TEST_F(ButContainerBitPackedSequence, UpTo8BitsSmokeTestOnHugeSequence)
{
  const std::vector<MaxElem> input{ MaxElem::X, MaxElem::Y };
  smokeTestPackingLongSequence<MaxPacker>(input);
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
