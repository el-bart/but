// TODO: remove once there's a real test with such a file
#include <But/detail/catch2.hpp>

namespace
{

TEST_CASE("But::placholder")
{
  // there must be at least one test, otherwise Catch2 v3 will fail the run
  REQUIRE(1 == 1);
}

}
