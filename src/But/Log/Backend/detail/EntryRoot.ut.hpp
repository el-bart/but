#pragma once
#include <gtest/gtest.h>
#include <But/Log/Backend/detail/EntryRoot.hpp>
#include <nlohmann/json.hpp>

using But::Log::Backend::detail::EntryRoot;
using But::Log::Backend::EntryProxy;
using But::Log::Backend::EntryArray;

namespace But::Log::Backend::detail
{

struct EntryRootTestBase: public testing::Test
{
  static auto unify(std::string const& in)
  {
    const auto tmp = nlohmann::json::parse(in);
    return tmp.dump(2);
  }

  EntryRoot er_;
};

#define EXPECT_EQ_JSON(a, b) \
        EXPECT_EQ( unify(a), unify( b.json() ) )

}
