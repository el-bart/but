#include <limits>
#include <gtest/gtest.h>
#include <But/Mpl/SizeTypeFor.hpp>

using But::Mpl::SizeTypeFor;

namespace
{

struct ButMplSizeTypeFor: public testing::Test
{
  static constexpr uint64_t eps = 42;
};


TEST_F(ButMplSizeTypeFor, DefaultsTo8Bits)
{
  EXPECT_TRUE( (std::is_same<uint8_t, SizeTypeFor<0u>::type>::value) );
}


TEST_F(ButMplSizeTypeFor, UpTo8Bits)
{
  EXPECT_TRUE( (std::is_same<uint8_t, SizeTypeFor< std::numeric_limits<uint8_t>::max()-eps >::type>::value) );
  EXPECT_TRUE( (std::is_same<uint8_t, SizeTypeFor< std::numeric_limits<uint8_t>::max()     >::type>::value) );
}


TEST_F(ButMplSizeTypeFor, UpTo16Bits)
{
  EXPECT_TRUE( (std::is_same<uint16_t, SizeTypeFor< std::numeric_limits< uint8_t>::max()+eps >::type>::value) );
  EXPECT_TRUE( (std::is_same<uint16_t, SizeTypeFor< std::numeric_limits<uint16_t>::max()     >::type>::value) );
}


TEST_F(ButMplSizeTypeFor, UpTo32Bits)
{
  EXPECT_TRUE( (std::is_same<uint32_t, SizeTypeFor< std::numeric_limits<uint16_t>::max()+eps >::type>::value) );
  EXPECT_TRUE( (std::is_same<uint32_t, SizeTypeFor< std::numeric_limits<uint32_t>::max()     >::type>::value) );
}


TEST_F(ButMplSizeTypeFor, UpTo64Bits)
{
  EXPECT_TRUE( (std::is_same<uint64_t, SizeTypeFor< std::numeric_limits<uint32_t>::max()+eps >::type>::value) );
  EXPECT_TRUE( (std::is_same<uint64_t, SizeTypeFor< std::numeric_limits<uint64_t>::max()     >::type>::value) );
}

}
