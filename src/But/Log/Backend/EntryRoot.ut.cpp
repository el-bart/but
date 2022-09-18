#include <gtest/gtest.h>
#include <But/Log/Backend/EntryRoot.hpp>
#include <nlohmann/json.hpp>

using But::Log::Backend::EntryRoot;

namespace
{

struct ButLogBackendEntryRoot: public testing::Test
{
  EntryRoot er_;
};

auto unify(std::string const& in)
{
  const auto tmp = nlohmann::ordered_json::parse(in);
  return tmp.dump(2);
}


TEST_F(ButLogBackendEntryRoot, EmptyJsonByDefault)
{
  EXPECT_EQ( "{}", er_.json() );
}

}
