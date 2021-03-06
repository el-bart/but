#include <gtest/gtest.h>
#include <But/Log/Backend/Tag.hpp>

using But::Log::Backend::Tag;

namespace
{

struct ButLogBackendTag: public testing::Test
{
  template<typename T>
  void checkNum(std::string const& exp)
  {
    T t{0};
    EXPECT_EQ( Tag::of(t), Tag{exp} );
  }

  template<typename T>
  void checkCharPtr(std::string const& exp)
  {
    T t{nullptr};
    EXPECT_EQ( Tag::of(t), Tag{exp} );
  }

  template<typename T>
  void checkCharArray(std::string const& exp)
  {
    T t[42] = "";
    EXPECT_EQ( Tag::of(t), Tag{exp} );
  }

  template<typename T>
  void checkStr(std::string const& exp)
  {
    T t{};
    EXPECT_EQ( Tag::of(t), Tag{exp} );
  }
};



TEST_F(ButLogBackendTag, CheckBasicValues)
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


TEST_F(ButLogBackendTag, AllCstringPointerTypesAreStdString)
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


TEST_F(ButLogBackendTag, AllCharArrayTypesAreStdString)
{
  checkCharArray<char               >("string");
  checkCharArray<char const         >("string");
  checkCharArray<char       volatile>("string");
  checkCharArray<char const volatile>("string");
}


TEST_F(ButLogBackendTag, StringIsSimplified)
{
  checkStr<std::string> ("string");
  checkStr<std::wstring>("string");
}


TEST_F(ButLogBackendTag, BooleanValue)
{
  checkStr<bool>("bool");
}


TEST_F(ButLogBackendTag, ToString)
{
  EXPECT_EQ( "bool", Tag::of(true).str() );
}

}
