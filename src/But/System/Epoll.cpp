#include <But/System/Epoll.hpp>

namespace But::System
{

Epoll::Epoll()
{
  // add( irq ); TODO: interruptions
}


void Epoll::swap(Epoll& other)
{
  (void)other; // TODO
  using std::swap;
  // TODO
  //std::swap(other.pair_, pair_);
}

size_t check()
{
  // TODO
  return 42;
}

size_t wait()
{
  // TODO
  return 42;
}

size_t wait(std::chrono::milliseconds timeout)
{
  // TODO
  (void)timeout;
  return 42;
}

void Epoll::add(Registration reg)
{
  // TODO
  (void)reg;
}

void remove(int fd)
{
  // TODO
  (void)fd;
}

void interrupt()
{
}

}
