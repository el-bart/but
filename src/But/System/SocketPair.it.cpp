#include <type_traits>
#include <But/System/SocketPair.hpp>
#include <catch2/catch.hpp>

using But::System::SocketPair;
using But::System::Descriptor;

namespace
{

SCENARIO("SocketPair: default-initialized")
{
  SocketPair sp;

  THEN("it's opened by default")
  {
    CHECK( sp.opened() );
    CHECK( sp );
    CHECK( sp.get().d1_.opened() );
    CHECK( sp.get().d2_.opened() );
  }

  WHEN("releasing")
  {
    auto const tmp = sp.release();
    CHECK(not sp);
    CHECK( tmp.d1_.opened() );
    CHECK( tmp.d2_.opened() );
  }

  WHEN("resetting with default value")
  {
    sp.reset();
    THEN("it's all closed")
    {
      CHECK( not sp );
      CHECK( not sp.get().d1_.opened() );
      CHECK( not sp.get().d2_.opened() );
    }
  }

  GIVEN("different socket pair set")
  {
    SocketPair other;
    int otherFd[2] = { other.get().d1_.get(), other.get().d2_.get() };
    int spFd[2]    = { sp.get().d1_.get(),    sp.get().d2_.get()    };

    WHEN("swapping")
    {
      using std::swap;
      swap(sp, other);

      THEN("descriptors are swapped")
      {
        CHECK( sp.get().d1_.get() == otherFd[0] );
        CHECK( sp.get().d2_.get() == otherFd[1] );
        CHECK( other.get().d1_.get() == spFd[0] );
        CHECK( other.get().d2_.get() == spFd[1] );
      }
    }

    WHEN("resetting with other value")
    {
      sp.reset( other.release() );

      THEN("other is closed")
      {
        CHECK(not other);
      }
      THEN("sp value has been updated to other's content")
      {
        CHECK( sp.get().d1_.get() == otherFd[0] );
        CHECK( sp.get().d2_.get() == otherFd[1] );
      }
    }

    WHEN("moving around")
    {
      sp = std::move(other);
      THEN("ownership is transfered")
      {
        CHECK(not other);
        CHECK(sp);
        CHECK( sp.get().d1_.get() == otherFd[0] );
        CHECK( sp.get().d2_.get() == otherFd[1] );
      }
    }

    WHEN("move-constructing")
    {
      SocketPair tmp{ std::move(sp) };
      THEN("ownership is transfered")
      {
        CHECK(not sp);
        CHECK(tmp);
        CHECK( tmp.get().d1_.get() == spFd[0] );
        CHECK( tmp.get().d2_.get() == spFd[1] );
      }
    }
  }
}


SCENARIO("SocketPair: is non-copyable")
{
  CHECK( not std::is_copy_assignable<SocketPair>::value );
  CHECK( not std::is_copy_constructible<SocketPair>::value );
}

}
