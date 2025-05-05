#include "allNamesUnique.hpp"
#include <gtest/gtest.h>

using But::Log::detail::allNamesUnique;

namespace
{

struct ButLogDetailAllNamesUnique: public testing::Test
{
  constexpr static std::string_view n1_{"one"};
  constexpr static std::string_view n2_{"two"};
  constexpr static std::string_view n3_{"three"};
};


TEST_F(ButLogDetailAllNamesUnique, EmptyArgumentsListIsOk)
{
  constexpr auto out = allNamesUnique();
  EXPECT_TRUE(out);
}


TEST_F(ButLogDetailAllNamesUnique, OneArgIsOk)
{
  constexpr auto out = allNamesUnique(n1_);
  EXPECT_TRUE(out);
}


TEST_F(ButLogDetailAllNamesUnique, MultipleUniqueAreOk)
{
  constexpr auto out = allNamesUnique(n1_, n2_, n3_);
  EXPECT_TRUE(out);
}


TEST_F(ButLogDetailAllNamesUnique, MultipleWithRepetitionsAreNotOk)
{
  {
    constexpr auto out = allNamesUnique(n1_, n1_);
    EXPECT_FALSE(out);
  }
  {
    constexpr auto out = allNamesUnique(n1_, n2_, n1_);
    EXPECT_FALSE(out);
  }
  {
    constexpr auto out = allNamesUnique(n3_, n2_, n1_, n1_);
    EXPECT_FALSE(out);
  }
  {
    constexpr auto out = allNamesUnique(n3_, n2_, n2_, n1_);
    EXPECT_FALSE(out);
  }
}


TEST_F(ButLogDetailAllNamesUnique, UnsetOptionalsAreNotIncludedInComparisons)
{
  constexpr std::optional<std::string_view> empty;
  ASSERT_FALSE(empty);
  {
    constexpr auto out = allNamesUnique(n1_, empty);
    EXPECT_TRUE(out);
  }
  {
    constexpr auto out = allNamesUnique(n1_, empty, n2_, empty);
    EXPECT_TRUE(out);
  }
}

}
