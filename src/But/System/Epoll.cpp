#include <But/System/Epoll.hpp>
#include <But/System/syscallRetry.hpp>
#include <stdexcept>
#include <sys/epoll.h>


namespace But::System
{

namespace
{
void drainFd(int fd, Epoll::Event)
{
  char buf[1024];
  while( read(fd, buf, sizeof(buf) ) > 0 )
  { }
}
}

Epoll::Epoll():
  epFd_{ syscallRetry( []() { return epoll_create1(0); } ) }
{
  if(not epFd_)
    BUT_THROW(EpollError, "failed to create: epoll_create1(): " << strerror(errno));
  add( irq_.get().d2_.get(), drainFd, Event::In );
}


void Epoll::swap(Epoll& other)
{
  (void)other; // TODO
  using std::swap;
  // TODO
  //std::swap(other.pair_, pair_);
}

size_t Epoll::check()
{
  // TODO
  return 42;
}

size_t Epoll::wait()
{
  // TODO
  return 42;
}

size_t Epoll::wait(std::chrono::milliseconds timeout)
{
  // TODO
  (void)timeout;
  return 42;
}

void Epoll::add(int fd, Registration &&reg)
{
  if(not reg.onEvent_)
    BUT_THROW(EpollError, "Epoll::add(): onEvent cannot be empty");
  // TODO
  (void)fd;
  (void)reg;
}

void Epoll::remove(int fd)
{
  // TODO
  (void)fd;
}

void Epoll::interrupt()
{
  if( syscallRetry( [&]() { return write( irq_.get().d1_.get(), "x", 1 ); } ) == -1 )
    BUT_THROW(EpollError, "write() to interrupt source socket pair failed: " << strerror(errno));
}

}
