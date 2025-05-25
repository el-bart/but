#include <But/System/Epoll.hpp>
#include <But/System/SocketPair.hpp>
#include <But/Threading/JoiningThread.hpp>
#include <thread>
#include <type_traits>
#include <catch2/catch.hpp>

using Clock = std::chrono::steady_clock;
using But::System::Epoll;
using But::System::SocketPair;
using But::Threading::JoiningThread;

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

  WHEN("adding negative FD")
  {
    THEN("exception is thrown")
    {
      CHECK_THROWS_AS( ep.add(-1, onFd1, Epoll::Event::In), Epoll::EpollError );
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

    AND_WHEN("2nd action on fd1 read is added")
    {
      auto otherCalls = 0u;
      auto otherAction = [&](int fd, Epoll::Event /*ev*/) { CHECK( fd == sp1.get().d1_.get() ); ++otherCalls; };
      ep.add( sp1.get().d1_.get(), otherAction, Epoll::Event::In );
      REQUIRE( write( sp1.get().d2_.get(), "foobar", 6 ) == 6 );
      THEN("check() runs both actions")
      {
        REQUIRE( ep.check() == 2 );
        CHECK( callsToFd1 == 1 );
        CHECK( otherCalls == 1 );
      }
    }
  }

  WHEN("fd1 is added to regeister more FDs, when its called")
  {
    auto addMore = [&](auto, auto) {
      ++callsToFd1;
      char buf[6];
      CHECK( read( sp1.get().d1_.get(), buf, 6 ) == 6 );
      ep.add( sp2.get().d1_.get(), onFd2, Epoll::Event::In );
      ep.add( sp3.get().d1_.get(), onFd3, Epoll::Event::In );
    };
    ep.add( sp1.get().d1_.get(), addMore, Epoll::Event::In );

    AND_WHEN("there's data waiting")
    {
      REQUIRE( write( sp1.get().d2_.get(), "foobar", 6 ) == 6 );
      THEN("check() returns actions")
      {
        REQUIRE( ep.check() == 1 );
        CHECK( callsToFd1 == 1 );

        AND_WHEN("data is waiting on fd2 and fd3")
        {
          REQUIRE( write( sp2.get().d2_.get(), "foo", 3 ) == 3 );
          REQUIRE( write( sp3.get().d2_.get(), "bar", 3 ) == 3 );
          THEN("check() returns actions")
          {
            REQUIRE( ep.check() == 2 );
            CHECK( callsToFd2 == 1 );
            CHECK( callsToFd3 == 1 );
          }
        }
      }
    }
  }

  WHEN("fd1 is added to regeister more actions on the same FD, when its called")
  {
    auto c1 = 0u;
    auto onC1 = [&](int /*fd*/, Epoll::Event /*ev*/) { ++c1; };
    auto c2 = 0u;
    auto onC2 = [&](int /*fd*/, Epoll::Event /*ev*/) { ++c2; };
    auto addMore = [&](auto, auto) {
      ++callsToFd1;
      char buf[6];
      CHECK( read( sp1.get().d1_.get(), buf, 6 ) == 6 );
      if( callsToFd1 == 1 )
      {
        // add more FDs - once
        ep.add( sp1.get().d1_.get(), onC1, Epoll::Event::Out );
        ep.add( sp1.get().d1_.get(), onC2, Epoll::Event::Out );
      }
    };
    ep.add( sp1.get().d1_.get(), addMore, Epoll::Event::In );

    AND_WHEN("there's data waiting")
    {
      REQUIRE( write( sp1.get().d2_.get(), "foobar", 6 ) == 6 );
      THEN("check() returns actions")
      {
        REQUIRE( ep.check() == 1 );
        CHECK( callsToFd1 == 1 );

        AND_WHEN("data can be written to fd1")
        {
          THEN("check() returns actions")
          {
            REQUIRE( ep.check() == 2 );
            CHECK( callsToFd1 == 1 );
            CHECK( c1 == 1 );
            CHECK( c2 == 1 );
          }
        }
      }
    }
  }

  WHEN("fd1 is added to remove fd1")
  {
    auto c1 = 0u;
    auto onC1 = [&](int /*fd*/, Epoll::Event /*ev*/) {
      ++c1;
      ep.remove( sp2.get().d1_.get() );
      char buf[6];
      CHECK( read( sp1.get().d1_.get(), buf, 6 ) == 6 );
    };
    ep.add( sp1.get().d1_.get(), onC1, Epoll::Event::In );
    AND_WHEN("fd2 is added")
    {
      ep.add( sp2.get().d1_.get(), onFd2, Epoll::Event::In );

      AND_WHEN("there's data waiting")
      {
        REQUIRE( write( sp1.get().d2_.get(), "foobar", 6 ) == 6 );
        THEN("check() returns actions")
        {
          REQUIRE( ep.check() == 1 );
          CHECK( c1 == 1 );
          CHECK( callsToFd2 == 0 );

          AND_WHEN("data is waiting on fd2")
          {
            REQUIRE( write( sp2.get().d2_.get(), "foobar", 6 ) == 6 );
            THEN("nothing happens")
            {
              REQUIRE( ep.check() == 0 );
            }
          }
        }
      }
    }
  }

  WHEN("fd1 is added with multiple actions, to remove via 1st callback one")
  {
    auto c1 = 0u;
    auto onC1 = [&](int /*fd*/, Epoll::Event /*ev*/) { ++c1; ep.remove( sp1.get().d1_.get() ); };
    auto c2 = 0u;
    auto onC2 = [&](int /*fd*/, Epoll::Event /*ev*/) { ++c2; };
    ep.add( sp1.get().d1_.get(), onC1, Epoll::Event::In );
    ep.add( sp1.get().d1_.get(), onC2, Epoll::Event::In );

    AND_WHEN("there's data waiting")
    {
      REQUIRE( write( sp1.get().d2_.get(), "foobar", 6 ) == 6 );
      THEN("check() calls all actions for a given FD")
      {
        REQUIRE( ep.check() == 2 );
        CHECK( c1 == 1 );
        CHECK( c2 == 1 );

        AND_WHEN("data is still waiting on fd1")
        {
          THEN("check() runs no more actions")
          {
            REQUIRE( ep.check() == 0 );
          }
        }
      }
    }
  }

  // TODO: re-register the same FD
  // TODO: closed FD and call check()

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

    AND_WHEN("FD is removed")
    {
      ep.remove( sp1.get().d1_.get() );
      AND_WHEN("data is written")
      {
        REQUIRE( write( sp1.get().d2_.get(), "foobar", 6 ) == 6 );
        THEN("no events are generated")
        {
          CHECK( ep.check() == 0 );
        }
      }
    }

    AND_WHEN("more than one action is registered to the same FD")
    {
      ep.add( sp1.get().d1_.get(), onFd1, Epoll::Event::Out );
      AND_WHEN("FD is removed")
      {
        ep.remove( sp1.get().d1_.get() );
        AND_WHEN("data is written")
        {
          REQUIRE( write( sp1.get().d2_.get(), "foobar", 6 ) == 6 );
          THEN("no events are generated")
          {
            CHECK( ep.check() == 0 );
          }
        }
      }
    }
  }

  WHEN("multiple FDs are added")
  {
    ep.add( sp1.get().d1_.get(), onFd1, Epoll::Event::In, Epoll::Event::Out );

    AND_WHEN("there's no data")
    {
      THEN("input is executed")
      {
        CHECK( ep.check() == 1 );
        CHECK( callsToFd1 == 1 );
      }
    }

    AND_WHEN("there's data waiting")
    {
      REQUIRE( write( sp1.get().d2_.get(), "foobar", 6 ) == 6 );
      THEN("both read and write actions are called")
      {
        REQUIRE( ep.check() == 2 );
        CHECK( callsToFd1 == 2 );
      }
    }
  }

  WHEN("interruption is called from a separate thread")
  {
    ep.add( sp1.get().d1_.get(), onFd1, Epoll::Event::In );
    JoiningThread<std::thread> th{ [&] { ep.interrupt(); } };

    THEN("blocking wait() is interrupted")
    {
      CHECK( ep.wait() == 0 );
    }
  }

  WHEN("2 FDs are registered")
  {
    ep.add( sp1.get().d1_.get(), onFd1, Epoll::Event::In );
    ep.add( sp2.get().d1_.get(), onFd2, Epoll::Event::In );

    AND_WHEN("2nd epoll is created with 1 more FD")
    {
      Epoll other;
      other.add( sp3.get().d1_.get(), onFd3, Epoll::Event::In );

      WHEN("swap() is called")
      {
        swap(ep, other);
        AND_WHEN("FD1 has data")
        {
          REQUIRE( write(sp1.get().d2_.get(), "abc", 3) == 3 );
          THEN("ep has no events")
          {
            CHECK( ep.check() == 0 );
            CHECK( callsToFd1 == 0 );
          }
          THEN("other Epoll has events")
          {
            CHECK( other.check() == 1 );
            CHECK( callsToFd1 == 1 );
          }
        }
      }

      WHEN("swap() is called")
      {
        swap(ep, other);
        AND_WHEN("FD1 has data")
        {
          REQUIRE( write(sp1.get().d2_.get(), "abc", 3) == 3 );
          THEN("ep has no events")
          {
            CHECK( ep.check() == 0 );
            CHECK( callsToFd1 == 0 );
          }
          THEN("other Epoll has events")
          {
            CHECK( other.check() == 1 );
            CHECK( callsToFd1 == 1 );
          }
        }
      }
    }
  }
  WHEN("2 dynamically-allocated Epolls are created with distinct FDs are registered")
  {
    auto ep1 = std::make_unique<Epoll>();
    ep1->add( sp1.get().d1_.get(), onFd1, Epoll::Event::In );
    ep1->add( sp2.get().d1_.get(), onFd2, Epoll::Event::In );

    AND_WHEN("2nd Epoll is dynamically allocated")
    {
      auto ep2 = std::make_unique<Epoll>();
      ep2->add( sp3.get().d1_.get(), onFd3, Epoll::Event::In );

      AND_WHEN("move assignment is called")
      {
        *ep2 = std::move(*ep1);

        AND_WHEN("data is added to FD from source Epoll")
        {
          REQUIRE( write(sp1.get().d2_.get(), "abc", 3) == 3 );

          THEN("data is available for reading")
          {
            CHECK( ep2->check() == 1 );
            CHECK( callsToFd1 == 1 );
          }
        }
      }
    }

    AND_WHEN("move-ctor is called")
    {
      Epoll ep2{ std::move(*ep1) };

      AND_WHEN("data is added to FD")
      {
        REQUIRE( write(sp1.get().d2_.get(), "abc", 3) == 3 );

        THEN("data is available for reading")
        {
          CHECK( ep2.check() == 1 );
          CHECK( callsToFd1 == 1 );
        }
      }
    }
  }
}


SCENARIO("But::System::Epoll: is non-copyable")
{
  CHECK( not std::is_copy_assignable<Epoll>::value );
  CHECK( not std::is_copy_constructible<Epoll>::value );
}

}
