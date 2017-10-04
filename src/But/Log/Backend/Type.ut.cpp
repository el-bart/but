#include "gtest/gtest.h"
#include "Type.hpp"

using But::Log::Backend::Type;

namespace
{

struct ButLogBackendType: public testing::Test
{
  template<typename T>
  void checkNum(std::string const& exp)
  {
    T t{0};
    EXPECT_EQ( Type::of(t), Type{exp} );
  }

  template<typename T>
  void checkCharPtr(std::string const& exp)
  {
    T t{nullptr};
    EXPECT_EQ( Type::of(t), Type{exp} );
  }

  template<typename T>
  void checkCharArray(std::string const& exp)
  {
    T t[42] = "";
    EXPECT_EQ( Type::of(t), Type{exp} );
  }

  template<typename T>
  void checkStr(std::string const& exp)
  {
    T t{};
    EXPECT_EQ( Type::of(t), Type{exp} );
  }
};



TEST_F(ButLogBackendType, CheckBasicValues)
{
  checkNum<char>("string");
  checkNum<short>("int");
  checkNum<int>("int");
  checkNum<long>("int");
  checkNum<long long>("int");

  checkNum<unsigned char>("unsigned int");
  checkNum<unsigned short>("unsigned int");
  checkNum<unsigned int>("unsigned int");
  checkNum<unsigned long>("unsigned int");
  checkNum<unsigned long long>("unsigned int");

  checkNum<float>("double");
  checkNum<double>("double");
  checkNum<long double>("double");
}


TEST_F(ButLogBackendType, AllCstringPointerTypesAreStdString)
{
  checkCharPtr<char                *               >("string");
  checkCharPtr<char const          *               >("string");
  checkCharPtr<char       volatile *               >("string");
  checkCharPtr<char const volatile *               >("string");

  checkCharPtr<char                * const         >("string");
  checkCharPtr<char const          * const         >("string");
  checkCharPtr<char       volatile * const         >("string");
  checkCharPtr<char const volatile * const         >("string");

  checkCharPtr<char                *       volatile>("string");
  checkCharPtr<char const          *       volatile>("string");
  checkCharPtr<char       volatile *       volatile>("string");
  checkCharPtr<char const volatile *       volatile>("string");

  checkCharPtr<char                * const volatile>("string");
  checkCharPtr<char const          * const volatile>("string");
  checkCharPtr<char       volatile * const volatile>("string");
  checkCharPtr<char const volatile * const volatile>("string");
}


TEST_F(ButLogBackendType, AllCharArrayTypesAreStdString)
{
  checkCharArray<char               >("string");
  checkCharArray<char const         >("string");
  checkCharArray<char       volatile>("string");
  checkCharArray<char const volatile>("string");
}


TEST_F(ButLogBackendType, StringIsSimplified)
{
  checkStr<std::string> ("string");
  checkStr<std::wstring>("string");
}


TEST_F(ButLogBackendType, BooleanValue)
{
  checkStr<bool>("bool");
}


TEST_F(ButLogBackendType, ToString)
{
  EXPECT_EQ( "bool", Type::of(true).str() );
}

}
