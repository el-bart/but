#include <gtest/gtest.h>
#include "assert.hpp"

namespace
{

struct ButAssert: public testing::Test
{ };


TEST_F(ButAssert, RuntimeCheck)
{
  BUT_ASSERT(1!=2);
  BUT_ASSERT(true);
  BUT_ASSERT(true || false);
}


constexpr bool ensureAnswer(int n)
{
  (void)n;
  BUT_ASSERT(n==42);
  return true;
}

TEST_F(ButAssert, CompileTimeCheck)
{
  constexpr auto ret = ensureAnswer(42);
  EXPECT_TRUE(ret);
}


TEST_F(ButAssert, FailedAssertionAtRuntimeInDebug)
{
#ifdef NDEBUG
  ensureAnswer(13); // in non-debug modes the assertion does not work
#else
  EXPECT_DEATH( ensureAnswer(13), "n==42" );
#endif
}

}
