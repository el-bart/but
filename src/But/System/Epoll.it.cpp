#include <But/System/Epoll.hpp>
#include <But/System/SocketPair.hpp>
#include <type_traits>
#include <catch2/catch.hpp>

using Clock = std::chrono::steady_clock;
using But::System::Epoll;
using But::System::SocketPair;

namespace
{

SCENARIO("But::System::Epoll: functionality")
{
  Epoll ep;

  WHEN("interrupt is called many times")
  {
    for(auto i = 0; i < 1'024; ++i)
      REQUIRE_NOTHROW( ep.interrupt() );
    THEN("interrupt does not block and interruption is applied once")
    {
      REQUIRE_NOTHROW( ep.interrupt() );
      CHECK( ep.wait() == 0u );
    }
  }

  WHEN("no descriptors added")
  {
    AND_WHEN("interruption() is called")
    {
      ep.interrupt();
      THEN("wait() returns immediately")
      {
        CHECK( ep.wait() == 0u );
      }
    }
  }
}

SCENARIO("But::System::Epoll: default-initialized")
{
  Epoll ep;

  SocketPair sp1;
  auto callsToFd1 = 0u;
  auto onFd1 = [&](int fd, Epoll::Event /*ev*/) { CHECK( fd == sp1.get().d1_.get() ); ++callsToFd1; };

  SocketPair sp2;
  auto callsToFd2 = 0u;
  auto onFd2 = [&](int fd, Epoll::Event /*ev*/) { CHECK( fd == sp2.get().d1_.get() ); ++callsToFd2; };

  SocketPair sp3;
  auto callsToFd3 = 0u;
  auto onFd3 = [&](int fd, Epoll::Event /*ev*/) { CHECK( fd == sp3.get().d1_.get() ); ++callsToFd3; };

  WHEN("nothing is added")
  {
    THEN("check() returns immediately")
    {
      CHECK( ep.check() == 0 );
    }
  }

  WHEN("fd1 is added for read")
  {
    ep.add( sp1.get().d1_.get(), onFd1, Epoll::Event::In );

    AND_WHEN("there's no data")
    {
      THEN("check() returns immidiately")
      {
        CHECK( ep.check() == 0 );
      }
    }

    AND_WHEN("there's data waiting")
    {
      REQUIRE( write( sp1.get().d2_.get(), "foobar", 6 ) == 6 );
      THEN("check() returns actions")
      {
        REQUIRE( ep.check() == 1 );
        CHECK( callsToFd1 == 1 );
      }
    }

    AND_WHEN("fd1 is added for write")
    {
      ep.add( sp1.get().d1_.get(), onFd1, Epoll::Event::Out );
      AND_WHEN("there's data waiting")
      {
        REQUIRE( write( sp1.get().d2_.get(), "foobar", 6 ) == 6 );
        THEN("check() returns actions for both read and write")
        {
          REQUIRE( ep.check() == 2 );
          CHECK( callsToFd1 == 2 );
        }
      }
    }
  }

  WHEN("multiple FDs are added")
  {
    ep.add( sp1.get().d1_.get(), onFd1, Epoll::Event::In );
    ep.add( sp2.get().d1_.get(), onFd2, Epoll::Event::In );
    ep.add( sp3.get().d1_.get(), onFd3, Epoll::Event::In );

    AND_WHEN("there's no data")
    {
      THEN("check() returns immidiately")
      {
        CHECK( ep.check() == 0 );
      }

      AND_WHEN("wait() with a timeout is called")
      {
        auto const timeout = std::chrono::milliseconds{3};
        auto const start = Clock::now();
        REQUIRE( ep.wait(timeout) == 0 );
        auto const dt = Clock::now() - start;
        THEN("call times out")
        {
          CHECK( dt >= timeout );
        }
      }
    }

    AND_WHEN("there's data waiting")
    {
      REQUIRE( write( sp2.get().d2_.get(), "foobar", 6 ) == 6 );
      THEN("check() returns actions for a proper FD")
      {
        REQUIRE( ep.check() == 1 );
        CHECK( callsToFd2 == 1 );
      }
    }
  }

  // TODO: add() multiple actions in 1 go
  // TODO: wait() with timeout
  // TODO: wait() with no timeout
  // TODO: remove()
  // TODO: remove() when >1 action is registered
  // TODO: swap
  // TODO: move-ctor
}


SCENARIO("But::System::Epoll: is non-copyable")
{
  CHECK( not std::is_copy_assignable<Epoll>::value );
  CHECK( not std::is_copy_constructible<Epoll>::value );
}

}
