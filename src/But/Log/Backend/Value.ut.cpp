#include <boost/type_index.hpp>
#include "gtest/gtest.h"
#include "Value.hpp"

using But::Log::Backend::Value;

namespace
{

template<typename T>
struct TestingVisitor
{
  void operator()(T const&) const { /* ok! */ }

  template<typename U>
  void operator()(U const&) const
  {
    FAIL() << "expected type: " << boost::typeindex::type_id<T>().pretty_name() << " - "
           << "got: " << boost::typeindex::type_id<U>().pretty_name();
  }
};


struct ButLogBackendValue: public testing::Test
{
  template<typename T>
  void checkSignedNum()
  {
    T t{0};
    const Value v{t};
    v.visit( TestingVisitor<int64_t>{} );
  }

  template<typename T>
  void checkUnsignedNum()
  {
    T t{0u};
    const Value v{t};
    v.visit( TestingVisitor<uint64_t>{} );
  }

  template<typename T>
  void checkFpNum()
  {
    T t{0.0};
    const Value v{t};
    v.visit( TestingVisitor<double>{} );
  }

  void checkBool()
  {
    auto b = false;
    const Value v{b};
    v.visit( TestingVisitor<bool>{} );
  }

  template<typename T>
  void checkCharPtr()
  {
    char buf[] = "text";
    T t{buf};
    const Value v{t};
    v.visit( TestingVisitor<std::string>{} );
  }

  template<typename T>
  void checkCharArray()
  {
    T t[42] = "test";
    const Value v{t};
    v.visit( TestingVisitor<std::string>{} );
  }

  template<typename T>
  void checkStr()
  {
    T t{};
    const Value v{t};
    v.visit( TestingVisitor<std::string>{} );
  }
};


TEST_F(ButLogBackendValue, CheckBasicValues)
{
  checkSignedNum<short>();
  checkSignedNum<int>();
  checkSignedNum<long>();
  checkSignedNum<long long>();

  checkUnsignedNum<unsigned char>();
  checkUnsignedNum<unsigned short>();
  checkUnsignedNum<unsigned int>();
  checkUnsignedNum<unsigned long>();
  checkUnsignedNum<unsigned long long>();

  checkFpNum<float>();
  checkFpNum<double>();
  checkFpNum<long double>();

  checkBool();
}


TEST_F(ButLogBackendValue, AllCstringPointerTypesAreStdString)
{
  checkCharPtr<char       *>();
  checkCharPtr<char const *>();

  checkCharPtr<char       * const>();
  checkCharPtr<char const * const>();
}


TEST_F(ButLogBackendValue, AllCharArrayTypesAreStdString)
{
  checkCharArray<char      >();
  checkCharArray<char const>();
}


TEST_F(ButLogBackendValue, StringIsSimplified)
{
  checkStr<char>       ();
  checkStr<std::string>();
  //checkStr<std::wstring>();
}


TEST_F(ButLogBackendValue, GettingValidType)
{
  EXPECT_EQ( 42u, Value{ uint16_t{42} }.get<uint64_t>() );
  EXPECT_EQ( 42u, Value{ uint32_t{42} }.get<uint64_t>() );
  EXPECT_EQ( 42u, Value{ uint64_t{42} }.get<uint64_t>() );

  EXPECT_EQ( 42, Value{ int16_t{42} }.get<int64_t>() );
  EXPECT_EQ( 42, Value{ int32_t{42} }.get<int64_t>() );
  EXPECT_EQ( 42, Value{ int64_t{42} }.get<int64_t>() );

  EXPECT_EQ( 4.5, Value{ float{4.5f} }.get<double>() );
  EXPECT_EQ( 4.5, Value{ double{4.5} }.get<double>() );
  {
    const long double ld = 4.5;
    EXPECT_EQ( 4.5, Value{ld}.get<double>() );
  }

  EXPECT_EQ( "ok", Value{"ok"}.get<std::string>() );
  EXPECT_EQ( "ok", Value{ std::string{"ok"} }.get<std::string>() );
}


TEST_F(ButLogBackendValue, Visiting)
{
  TestingVisitor<int64_t> tv{};

  {
    Value v{42};
    v.visit(tv);
  }
  {
    const Value v{42};
    v.visit(tv);
  }
  {
    Value v{42};
    std::move(v).visit(tv);
  }
}


TEST_F(ButLogBackendValue, Comparisons)
{
  const Value i1{1};
  const Value i2{2};

  const Value ui{1u};
  const Value d{4.2};
  const Value s{"alice"};
  const Value b{true};

  // positive tests
  EXPECT_TRUE( i1 <  i2 );
  EXPECT_TRUE( i1 <= i1 );

  EXPECT_TRUE( i2 >  i1 );
  EXPECT_TRUE( i2 >= i1 );

  EXPECT_TRUE( i1 == i1 );
  EXPECT_TRUE( i1 != i2 );

  // negative tests
  EXPECT_FALSE( i1 > i2 );
  EXPECT_FALSE( i2 < i1 );

  EXPECT_FALSE( i1 != i1 );
  EXPECT_FALSE( i1 == i2 );

  // type-missmatch
  EXPECT_FALSE( i1 == ui );
  EXPECT_FALSE( i1 == d );
  EXPECT_FALSE( i1 == s );
  EXPECT_FALSE( i1 == b );
}


TEST_F(ButLogBackendValue, Streamming)
{
  {
    std::stringstream ss;
    ss << Value{42};
    EXPECT_EQ( "42", ss.str() );
  }
  {
    std::stringstream ss;
    ss << Value{42u};
    EXPECT_EQ( "42", ss.str() );
  }
  {
    std::stringstream ss;
    ss << Value{4.2};
    EXPECT_EQ( "4.2", ss.str() );
  }
  {
    std::stringstream ss;
    ss << Value{'x'};
    EXPECT_EQ( "x", ss.str() );
  }
  {
    std::stringstream ss;
    ss << Value{"foo"};
    EXPECT_EQ( "foo", ss.str() );
  }
}

}
