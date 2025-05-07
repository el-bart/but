#include <But/System/syscallRetry.hpp>
#include <catch2/catch.hpp>

using But::System::syscallRetry;

namespace
{

SCENARIO("syscallRetry(): functionality")
{
  WHEN("running successful syscall with positive value")
  {
    auto counter = 0;
    auto const ret = syscallRetry( [&]() { ++counter; return 42; } );
    THEN("it's called once")
    {
      CHECK(counter == 1);
      CHECK(ret == 42);
    }
  }

  WHEN("running successful syscall with value == 0")
  {
    auto counter = 0;
    auto const ret = syscallRetry( [&]() { ++counter; return 0; } );
    THEN("it's called once")
    {
      CHECK(counter == 1);
      CHECK(ret == 0);
    }
  }

  WHEN("running and returning -1 and errno of non-known value")
  {
    auto counter = 0;
    auto const ret = syscallRetry( [&]() { ++counter; errno=EPERM; return -1; } );
    THEN("it's called once and returns error")
    {
      CHECK(counter == 1);
      CHECK(ret == -1);
      CHECK(errno == EPERM);
    }
  }

  WHEN("running and returning -1 and errno EINTR")
  {
    auto counter = 0;
    auto const ret = syscallRetry( [&]() {
        if(++counter < 3)
        {
          errno=EINTR;
          return -1;
        }
        errno=0;
        return 42;
        } );
    THEN("it's retried until non-error is returned")
    {
      CHECK(counter == 3);
      CHECK(ret == 42);
      CHECK(errno == 0);
    }
  }

  WHEN("running and returning -1 and errno EAGAIN")
  {
    auto counter = 0;
    auto const ret = syscallRetry( [&]() {
        if(++counter < 3)
        {
          errno=EAGAIN;
          return -1;
        }
        errno=0;
        return 42;
        } );
    THEN("it's retried until non-error is returned")
    {
      CHECK(counter == 3);
      CHECK(ret == 42);
      CHECK(errno == 0);
    }
  }
}

}
