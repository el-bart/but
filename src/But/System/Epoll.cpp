#include <But/System/Epoll.hpp>

namespace But::System
{

Epoll::Epoll()
{
  // add( irq ); TODO: interruptions
}

void Epoll::add(Registration reg)
{
  (void)reg;
}

}
