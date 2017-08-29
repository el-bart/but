#include <gtest/gtest.h>
#include "ConstexprAssert.hpp"

namespace
{

struct ButConstexprAssert: public testing::Test
{ };


TEST_F(ButConstexprAssert, RuntimeCheck)
{
  BUT_CONSTEXPR_ASSERT(1!=2);
  BUT_CONSTEXPR_ASSERT(true);
  BUT_CONSTEXPR_ASSERT(true || false);
}


constexpr bool ensureAnswer(int n)
{
  BUT_CONSTEXPR_ASSERT(n==42);
  return true;
}

TEST_F(ButConstexprAssert, CompileTimeCheck)
{
  constexpr auto ret = ensureAnswer(42);
  EXPECT_TRUE(ret);
}


TEST_F(ButConstexprAssert, FailedAssertionAtRuntime)
{
  EXPECT_DEATH( ensureAnswer(13), "n==42" );
}

}
