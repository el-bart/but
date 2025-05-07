#pragma once
#include <cerrno>

namespace But::System
{

/** @brief simple wrapper to retry syscalls that return known non-errors that require syscall restart.
 *  @param f functor to run. e.g.: [&]() { return read(fd, buf, sizeof(buf); }
 *  @returns final result from syscall.
 */
template<typename F>
int syscallRetry(F&& f)
{
  errno = 0;
  while(true)
  {
    auto const ret = f();
    if(ret != -1)
      return ret;
    switch(errno)
    {
      case EINTR:  continue;    // interrupted system call
      case EAGAIN: continue;    // resource temporary unavailable
      default: return ret;
    }
  }
}

}
