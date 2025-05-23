#include <type_traits>
#include <But/System/makeNonblocking.hpp>
#include <But/System/SocketPair.hpp>
#include <catch2/catch.hpp>

using But::System::SocketPair;
using But::System::makeNonblocking;
using But::System::CannotMarkSocketNonblocking;

namespace
{

TEST_CASE("But::System::makeNonblocking()")
{
  CHECK_THROWS_AS( makeNonblocking(-1), CannotMarkSocketNonblocking );
  SocketPair sp;
  auto const fd1 = sp.get().d1_.get();
  auto const fd2 = sp.get().d2_.get();
  makeNonblocking(fd1);

  SECTION("read() when there's no data")
  {
    char buf[10];
    REQUIRE( read(fd1, buf, 1) == -1 );
    CHECK( errno == EWOULDBLOCK );
  }

  SECTION("read() when there's some data, but less then required")
  {
    REQUIRE( write(fd2, "foo", 3) == 3 );
    char buf[10];
    bzero(buf, 10);
    REQUIRE( read(fd1, buf, 10) == 3 );
    CHECK( buf == std::string_view{"foo"} );
  }
}

}
