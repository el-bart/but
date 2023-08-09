#pragma once
#include <gtest/gtest.h>
#include <But/Log/Backend/detail/EntryRoot.hpp>
#include <nlohmann/json.hpp>

namespace But::Log::Backend::detail
{

struct EntryRootTestBase: public testing::Test
{
  static std::string unify(char const* in)
  {
    return unify( nlohmann::json::parse(in) );
  }
  static std::string unify(std::string const& in)
  {
    return unify( nlohmann::json::parse(in) );
  }
  static std::string unify(nlohmann::json const& in)
  {
    return in.dump(4);
  }
  static std::string unify(But::Log::Backend::detail::EntryRoot const& in)
  {
    return unify( in.json() );
  }

  But::Log::Backend::detail::EntryRoot er_;
};

#define EXPECT_EQ_JSON(a, b) \
        EXPECT_EQ( unify(a), unify(b) )

}
