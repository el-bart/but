#pragma once
#include <gtest/gtest.h>
#include <But/Log/Backend/EntryRoot.hpp>
#include <But/Log/Backend/unifyJson.utg.hpp>

namespace But::Log::Backend
{

struct EntryRootTestBase: public testing::Test
{
  EntryRoot er_;
};

}
