#pragma once
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <But/Exception.hpp>
#include <But/System/syscallRetry.hpp>

namespace But::System
{

BUT_DEFINE_EXCEPTION(CannotMarkSocketNonblocking, Exception, "cannot mark socket as non-blocking");

inline void makeNonblocking(int const fd)
{
  auto const flags = syscallRetry( [&]() { return fcntl(fd, F_GETFL, 0); } );
  if(flags == -1)
    BUT_THROW(CannotMarkSocketNonblocking, "fcntl(F_GETFL) on fd=" << fd);

  if( syscallRetry( [&]() { return fcntl(fd, F_SETFL, flags|O_NONBLOCK); } ) == -1 )
    BUT_THROW(CannotMarkSocketNonblocking, "fcntl(F_SETFL) on fd=" << fd);
}

}
