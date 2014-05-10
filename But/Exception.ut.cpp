#include <limits>

#include "gtest/gtest.h"
#include "Exception.hpp"

using But::Exception;

namespace
{

struct ButThreadingException: public testing::Test
{ };


TEST_F(ButThreadingException, SomeMessage)
{
  const Exception ex{"file.hpp", 42, "func()", "msg"};
  EXPECT_EQ(ex.what(), std::string{"file.hpp:42 msg (in func())"});
}


TEST_F(ButThreadingException, MaxLineNumber)
{
  constexpr auto max = std::numeric_limits<unsigned>::max();
  const Exception ex{"file.hpp", max, "func()", "msg"};
  std::stringstream ss;
  ss << "file.hpp:" << max << " msg (in func())";
  EXPECT_EQ(ex.what(), ss.str());
}

}
