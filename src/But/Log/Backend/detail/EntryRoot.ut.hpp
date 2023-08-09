#pragma once
#include <gtest/gtest.h>
#include <But/Log/Backend/detail/EntryRoot.hpp>
#include <But/Log/Backend/detail/unifyJson.ut.hpp>

namespace But::Log::Backend::detail
{

struct EntryRootTestBase: public testing::Test
{
  But::Log::Backend::detail::EntryRoot er_;
};

}
