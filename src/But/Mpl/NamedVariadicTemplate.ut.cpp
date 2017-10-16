#include <gtest/gtest.h>
#include <But/Mpl/NamedVariadicTemplate.hpp>

using But::Mpl::NamedVariadicTemplate;

namespace
{

struct ButMplNamedVariadicTemplate: public testing::Test
{ };


template<typename ...Args>
struct MyUsage;

template<typename ...Args>
struct MyUsage< NamedVariadicTemplate<Args...> >
{
  static constexpr auto size() { return sizeof...(Args); }
};


TEST_F(ButMplNamedVariadicTemplate, KeepingArguments)
{
  using Store = NamedVariadicTemplate<int,double>;
  EXPECT_EQ( 2u, MyUsage<Store>::size() );
}


TEST_F(ButMplNamedVariadicTemplate, GettingArgumentsPackSize)
{
  EXPECT_EQ( 0u, ( NamedVariadicTemplate<>::size() ) );
  EXPECT_EQ( 2u, ( NamedVariadicTemplate<int,double>::size() ) );
  EXPECT_EQ( 4u, ( NamedVariadicTemplate<int,double,int,void*>::size() ) );
}


TEST_F(ButMplNamedVariadicTemplate, GettingSizeIsConstexpr)
{
  constexpr auto size = NamedVariadicTemplate<int,double>::size();
  EXPECT_EQ( 2u, size );
}

}
