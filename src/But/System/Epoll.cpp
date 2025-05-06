#include <But/System/Epoll.hpp>
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
  epFd_{ epoll_create1(0) }
{
  // TODO: POSIX retry thing...
  if(not epFd_)
    throw std::runtime_error{"But::System::Epoll: failed to create: epoll_create1() failed"}; // TODO errno info
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
    throw std::logic_error{"But::System::Epoll::add(): onEvent cannot be empty"};
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
  // TODO: retries on POSIX stuff
  write( irq_.get().d1_.get(), "x", 1 );
}

}
