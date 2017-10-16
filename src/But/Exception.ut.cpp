#include <limits>
#include <typeinfo>
#include <type_traits>
#include <boost/exception/all.hpp>

#include <gtest/gtest.h>
#include <But/Exception.hpp>

using But::Exception;

namespace
{

// helper needed, since original c-tor is protected, as class is not to be used as a standalone object
BUT_DEFINE_EXCEPTION(TestEx, Exception, "default");

struct ButException: public testing::Test
{ };


TEST_F(ButException, SomeMessage)
{
  const TestEx ex{"file.hpp", 42u, "func()", "msg"};
  EXPECT_EQ(ex.what(), std::string{"file.hpp:42 default: msg (in func())"});
}


TEST_F(ButException, SomeMessageWithStdString)
{
  const TestEx ex{"file.hpp", 42u, "func()", std::string{"msg"}};
  EXPECT_EQ(ex.what(), std::string{"file.hpp:42 default: msg (in func())"});
}


TEST_F(ButException, MaxLineNumber)
{
  constexpr auto max = std::numeric_limits<unsigned>::max();
  const TestEx ex{"file.hpp", max, "func()", "msg"};
  std::stringstream ss;
  ss << "file.hpp:" << max << " default: msg (in func())";
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
  catch(Problem const& ex)
  {
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
  BUT_DEFINE_EXCEPTION(TopProblem, NewProblem, "last");
  EXPECT_TRUE( (std::is_base_of<Problem, NewProblem>::value) );

  try
  {
    BUT_THROW(TopProblem, "whatever " << 10);
    FAIL() << "no exception has been thrown";
  }
  catch(TopProblem const& ex)
  {
    const std::string m = ex.what();
    EXPECT_TRUE( m.find(__FILE__) != std::string::npos ) << "actuall got: " << m;
    EXPECT_TRUE( m.find(BOOST_CURRENT_FUNCTION) != std::string::npos ) << "actuall got: " << m;
    EXPECT_TRUE( m.find("base: derived: last: whatever 10") != std::string::npos ) << "actuall got: " << m;
  }

  try
  {
    BUT_THROW(TopProblem, "whatever " << 10);
    FAIL() << "no exception has been thrown";
  }
  catch(NewProblem const&)
  {
    // ok
  }

  try
  {
    BUT_THROW(TopProblem, "whatever " << 10);
    FAIL() << "no exception has been thrown";
  }
  catch(Problem const&)
  {
    // ok
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


TEST_F(ButException, ForwardingExactExceptionTypeWithStd)
{
  BUT_DEFINE_EXCEPTION(Sth, Exception, "sth");
  try
  {
    BUT_THROW(Sth, "test");
  }
  catch(...)
  {
    try
    {
      std::rethrow_exception( std::current_exception() );
    }
    catch(Sth const&)
    {
      return; // ok
    }
  }
  FAIL() << "oops...";
}


TEST_F(ButException, ForwardingExactExceptionTypeWithBoost)
{
  BUT_DEFINE_EXCEPTION(Sth, Exception, "sth");
  try
  {
    BUT_THROW(Sth, "test");
  }
  catch(...)
  {
    try
    {
      boost::rethrow_exception( boost::current_exception() );
    }
    catch(Sth const&)
    {
      return; // ok
    }
  }
  FAIL() << "oops...";
}

}
