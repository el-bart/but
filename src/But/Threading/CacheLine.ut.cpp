#include "gtest/gtest.h"
#include "CacheLine.hpp"

using But::Threading::CacheLine;

namespace
{

struct ButThreadingCacheLine: public testing::Test
{ };


TEST_F(ButThreadingCacheLine, AlignmentSmallType)
{
  CacheLine<int, 128> n;
  const auto addr = reinterpret_cast<unsigned long long>(&n);
  EXPECT_TRUE( (addr%128) == 0 );
}


TEST_F(ButThreadingCacheLine, PaddingSmallType)
{
  EXPECT_TRUE( sizeof( CacheLine<int, 128> ) >= 128 );
}


struct BigType
{
  char sth_[500]; // NOTE: must be bigger than cache line size used in test
};

TEST_F(ButThreadingCacheLine, AlignmentBigType)
{
  CacheLine<BigType, 128> n;
  const auto addr = reinterpret_cast<unsigned long long>(&n);
  EXPECT_TRUE( (addr%128) == 0 );
}


TEST_F(ButThreadingCacheLine, PaddingBigType)
{
  EXPECT_TRUE( sizeof( CacheLine<BigType, 128> ) >= 512 );
}


TEST_F(ButThreadingCacheLine, Construction)
{
  CacheLine<int, 128> a{666};
  EXPECT_EQ(666, a.data_);
}


TEST_F(ButThreadingCacheLine, CopyConstruction)
{
  CacheLine<int, 128> a{666};
  CacheLine<int, 128> b(a);
  EXPECT_EQ(a.data_, b.data_);
}


TEST_F(ButThreadingCacheLine, CopyAssignment)
{
  CacheLine<int, 128> a{666};
  CacheLine<int, 128> b{42};
  b = a;
  EXPECT_EQ(a.data_, b.data_);
}


bool isPowerOf2(const unsigned n)
{
  if(n<=2)
    return true;
  if(n%2)
    return false;
  return isPowerOf2(n/2);
}

TEST_F(ButThreadingCacheLine, IsPowerOf2SanityCheck)
{
  EXPECT_TRUE(  isPowerOf2(1) );
  EXPECT_TRUE(  isPowerOf2(2) );
  EXPECT_FALSE( isPowerOf2(3) );
  EXPECT_TRUE(  isPowerOf2(4) );
  EXPECT_FALSE( isPowerOf2(5) );
  EXPECT_FALSE( isPowerOf2(6) );
  EXPECT_FALSE( isPowerOf2(7) );
  EXPECT_TRUE(  isPowerOf2(8) );
  EXPECT_FALSE( isPowerOf2(9) );
}

TEST_F(ButThreadingCacheLine, DefaultCacheLineSize)
{
  EXPECT_TRUE( isPowerOf2( alignof(CacheLine<int>) ) );
}


TEST_F(ButThreadingCacheLine, ValueType)
{
  EXPECT_TRUE( (std::is_same<CacheLine<int>::value_type, int>::value) );
}


TEST_F(ButThreadingCacheLine, CacheLineSizeInfo)
{
  EXPECT_EQ( 32u, alignof(CacheLine<int, 32>) );
}


struct NonTrivialCtor
{
  int answer_ = 42;
};

TEST_F(ButThreadingCacheLine, NonTrivialConstructor)
{
  CacheLine<NonTrivialCtor> ntc;
}

}
