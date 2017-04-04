#include <limits>
#include <typeinfo>
#include <type_traits>

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

struct ButException: public testing::Test
{ };


TEST_F(ButException, SomeMessage)
{
  const TestEx ex{"file.hpp", 42u, "func()", "defMsg", "msg"};
  EXPECT_EQ(ex.what(), std::string{"file.hpp:42 defMsg: msg (in func())"});
}


TEST_F(ButException, MaxLineNumber)
{
  constexpr auto max = std::numeric_limits<unsigned>::max();
  const TestEx ex{"file.hpp", max, "func()", "defMsg", "msg"};
  std::stringstream ss;
  ss << "file.hpp:" << max << " defMsg: msg (in func())";
  EXPECT_EQ(ex.what(), ss.str());
}


TEST_F(ButException, DefiningAndThrowing)
{
  BUT_DEFINE_EXCEPTION(Problem, Exception, "holy shit");
  try
  {
    BUT_THROW(Problem, "what the hell is that? " << 52 << "? should be " << 2*21 << "!");
    FAIL() << "no exception has been thrown";
  }
  catch(Exception const& ex)
  {
    EXPECT_EQ( std::string(typeid(ex).name()), typeid(Problem).name() );
    const std::string m = ex.what();
    EXPECT_TRUE( m.find(__FILE__) != std::string::npos ) << "actuall got: " << m;
    EXPECT_TRUE( m.find(BOOST_CURRENT_FUNCTION) != std::string::npos ) << "actuall got: " << m;
    EXPECT_TRUE( m.find("holy shit: what the hell is that? 52? should be 42!") != std::string::npos ) << "actuall got: " << m;
  }
}


TEST_F(ButException, MultipleDeriving)
{
  BUT_DEFINE_EXCEPTION(Problem, Exception, "base");
  BUT_DEFINE_EXCEPTION(NewProblem, Problem, "derived");
  EXPECT_TRUE( (std::is_base_of<Problem, NewProblem>::value) );
  try
  {
    BUT_THROW(NewProblem, "whatever " << 10);
    FAIL() << "no exception has been thrown";
  }
  catch(NewProblem const& ex)
  {
    EXPECT_EQ( std::string(typeid(ex).name()), typeid(NewProblem).name() );
    const std::string m = ex.what();
    EXPECT_TRUE( m.find(__FILE__) != std::string::npos ) << "actuall got: " << m;
    EXPECT_TRUE( m.find(BOOST_CURRENT_FUNCTION) != std::string::npos ) << "actuall got: " << m;
    EXPECT_TRUE( m.find("derived: whatever 10") != std::string::npos ) << "actuall got: " << m;
  }
}


TEST_F(ButException, DefiningAndThrowingWithEmptyMessage)
{
  BUT_DEFINE_EXCEPTION(Sth, Exception, "holy shit");
  try
  {
    BUT_THROW(Sth, "");
    FAIL() << "no exception has been thrown";
  }
  catch(Sth const&)
  {
  }
}


TEST_F(ButException, DefiningWithEmptyMessage)
{
  BUT_DEFINE_EXCEPTION(Sth, Exception, "");
  try
  {
    BUT_THROW(Sth, "");
    FAIL() << "no exception has been thrown";
  }
  catch(Sth const&)
  {
  }
}

}
