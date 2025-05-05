#include <gtest/gtest.h>
#include <But/Container/Array.hpp>

using But::Container::Array;

namespace
{

using Sequence = Array<int, 3>;

struct ButContainerArray: public testing::Test
{ };


TEST_F(ButContainerArray, ConstexprDefaultCtorSmokeTest)
{
  constexpr Sequence s{};
  EXPECT_EQ( 3u, s.size() );
  EXPECT_EQ( 3u, s.max_size() );
}


TEST_F(ButContainerArray, ConstexprInitialization)
{
  constexpr Sequence s{ {3,1,4} };
  EXPECT_EQ( 3, s[0] );
  EXPECT_EQ( 1, s[1] );
  EXPECT_EQ( 4, s[2] );
}


TEST_F(ButContainerArray, ConstexprSize)
{
  constexpr Sequence s{ {3,1,4} };
  constexpr auto size = s.size();
  EXPECT_EQ( 3u, size );
}


constexpr auto sum(Sequence const& s)
{
  auto out = 0;
  for(auto& e: s)
    out += e;
  return out;
}

TEST_F(ButContainerArray, ConstexprIteration)
{
  constexpr auto out = sum( Sequence{{1,2,3}} );
  EXPECT_EQ( 6, out );
}


constexpr auto mod(Sequence&& s)
{
  for(auto& e: s)
    e += 1;
  return s;
}

TEST_F(ButContainerArray, ConstexprIterationWithModification)
{
  constexpr auto s = mod( Sequence{{1,2,3}} );
  EXPECT_EQ( 2, s[0] );
  EXPECT_EQ( 3, s[1] );
  EXPECT_EQ( 4, s[2] );
}


constexpr auto changeSome(Sequence&& s)
{
  s[1] = 42;
  return s;
}

TEST_F(ButContainerArray, ConstexprSquareBracketOperator)
{
  constexpr auto s = changeSome( Sequence{{1,2,3}} );
  constexpr auto e0 = s[0];
  constexpr auto e1 = s[1];
  constexpr auto e2 = s[2];
  EXPECT_EQ( 1, e0 );
  EXPECT_EQ( 42, e1 );
  EXPECT_EQ( 3, e2 );
}


constexpr auto fill(Sequence&& s, int v)
{
  s.fill(v);
  return s;
}

TEST_F(ButContainerArray, ConstexprFill)
{
  constexpr auto s = fill( Sequence{{1,2,3}}, 42 );
  EXPECT_EQ( 42, s[0] );
  EXPECT_EQ( 42, s[1] );
  EXPECT_EQ( 42, s[2] );
}

}
