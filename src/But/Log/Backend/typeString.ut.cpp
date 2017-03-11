#include "gtest/gtest.h"
#include "typeString.hpp"

using But::Log::Backend::typeString;

namespace
{

struct ButLogBackendTypeInfo: public testing::Test
{
  template<typename T>
  void checkNum(std::string const& exp)
  {
    T t{0};
    EXPECT_EQ( typeString(t), exp );
  }

  template<typename T>
  void checkCharPtr(std::string const& exp)
  {
    T t{nullptr};
    EXPECT_EQ( typeString(t), exp );
  }

  template<typename T>
  void checkCharArray(std::string const& exp)
  {
    T t[42] = "";
    EXPECT_EQ( typeString(t), exp );
  }

  template<typename T>
  void checkStr(std::string const& exp)
  {
    T t{};
    EXPECT_EQ( typeString(t), exp );
  }
};



TEST_F(ButLogBackendTypeInfo, CheckBasicValues)
{
  checkNum<char>("char");
  checkNum<short>("short");
  checkNum<int>("int");
  checkNum<long>("long");

  checkNum<unsigned char>("unsigned char");
  checkNum<unsigned short>("unsigned short");
  checkNum<unsigned int>("unsigned int");
  checkNum<unsigned long>("unsigned long");

  checkNum<float>("float");
  checkNum<double>("double");
  checkNum<long double>("long double");
}


TEST_F(ButLogBackendTypeInfo, AllCstringPointerTypesAreStdString)
{
  checkCharPtr<char                *                >("std::string");
  checkCharPtr<char const          *                >("std::string");
  checkCharPtr<char       volatile *                >("std::string");
  checkCharPtr<char const volatile *                >("std::string");

  checkCharPtr<char                * const          >("std::string");
  checkCharPtr<char const          * const          >("std::string");
  checkCharPtr<char       volatile * const          >("std::string");
  checkCharPtr<char const volatile * const          >("std::string");

  checkCharPtr<char                *       volatile>("std::string");
  checkCharPtr<char const          *       volatile>("std::string");
  checkCharPtr<char       volatile *       volatile>("std::string");
  checkCharPtr<char const volatile *       volatile>("std::string");

  checkCharPtr<char                * const volatile>("std::string");
  checkCharPtr<char const          * const volatile>("std::string");
  checkCharPtr<char       volatile * const volatile>("std::string");
  checkCharPtr<char const volatile * const volatile>("std::string");
}


TEST_F(ButLogBackendTypeInfo, AllCharArrayTypesAreStdString)
{
  checkCharArray<char               >("std::string");
  checkCharArray<char const         >("std::string");
  checkCharArray<char       volatile>("std::string");
  checkCharArray<char const volatile>("std::string");
}


TEST_F(ButLogBackendTypeInfo, StringIsSimplified)
{
  checkStr<std::string> ("std::string");
  checkStr<std::wstring>("std::wstring");
}

}
