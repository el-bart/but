#include <limits>

#include "gtest/gtest.h"
#include "Exception.hpp"

using But::Exception;

namespace
{

// helper needed, since original c-tor is protected, as class is not to be used as a standalone object
struct TestEx: public Exception
{
  template<typename ...Args>
  explicit TestEx(Args&&... args):
    Exception{ std::forward<Args>(args)... }
  { }
};

struct ButThreadingException: public testing::Test
{ };


TEST_F(ButThreadingException, SomeMessage)
{
  const TestEx ex{"file.hpp", 42u, "func()", "msg"};
  EXPECT_EQ(ex.what(), std::string{"file.hpp:42 msg (in func())"});
}


TEST_F(ButThreadingException, MaxLineNumber)
{
  constexpr auto max = std::numeric_limits<unsigned>::max();
  const TestEx ex{"file.hpp", max, "func()", "msg"};
  std::stringstream ss;
  ss << "file.hpp:" << max << " msg (in func())";
  EXPECT_EQ(ex.what(), ss.str());
}

}
