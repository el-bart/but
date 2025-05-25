#include <But/System/Epoll.hpp>
#include <But/System/syscallRetry.hpp>
#include <But/System/makeNonblocking.hpp>
#include <stdexcept>
#include <cassert>


namespace But::System
{

namespace
{
inline void drainFd(int fd, Epoll::Event /*ev*/)
{
  char buf[1024];
  while( syscallRetry( [&]() { return read(fd, buf, sizeof(buf) ); } ) > 0 )
  { }
}
}

Epoll::Epoll():
  epFd_{ syscallRetry( []() { return epoll_create1(0); } ) }
{
  if(not epFd_)
    BUT_THROW(EpollError, "failed to create: epoll_create1(): " << strerror(errno));
  // make interruption sockets non-blocking, to make draining and interrupting easier
  makeNonblocking( interruptSource_.get().d1_.get() );
  makeNonblocking( interruptSource_.get().d2_.get() );
  add( interruptSource_.get().d2_.get(), drainFd, Event::In );
}


void Epoll::swap(Epoll& other)
{
  using std::swap;
  swap(interruptSource_, other.interruptSource_);
  swap(epFd_, other.epFd_);
  swap(registrations_, other.registrations_);
}


void Epoll::remove(int fd)
{
  auto it = registrations_.find(fd);
  if(it == end(registrations_))
    return;
  if( syscallRetry( [&]() { return epoll_ctl(epFd_.get(), EPOLL_CTL_DEL, fd, nullptr); } ) == -1 )
    BUT_THROW(EpollError, "epoll_ctr(EPOLL_CTL_DEL): failed to remove fd=" << fd << ": " << strerror(errno));
  registrations_.erase(it);
}


void Epoll::interrupt()
{
  if( syscallRetry( [&]() { return write( interruptSource_.get().d1_.get(), "x", 1 ); } ) != -1 )
    return;
  if( errno == EAGAIN)   // all good - there's already data in the socket so interruption will work fine
    return;
  BUT_THROW(EpollError, "write() to interrupt source socket pair failed: " << strerror(errno));
}


void Epoll::add(int fd, Registration &&reg)
{
  if(not reg.onEvent_)
    BUT_THROW(EpollError, "Epoll::add(): onEvent cannot be empty");

  auto it = registrations_.find(fd);
  if(it == end(registrations_))
    addNew(fd, std::move(reg));
  else
    addToExisting(it, std::move(reg));
}


void Epoll::addNew(int fd, Registration &&reg)
{
  auto& v = registrations_[fd];
  assert(v.empty());

  epoll_event ev;
  bzero(&ev, sizeof(ev));
  ev.data.fd = fd;
  ev.events = reg.events_;
  if( syscallRetry( [&]() { return epoll_ctl(epFd_.get(), EPOLL_CTL_ADD, fd, &ev); } ) == -1 )
    BUT_THROW(EpollError, "epoll_ctr(EPOLL_CTL_ADD): failed to add fd=" << fd << ": " << strerror(errno));

  v.push_back( std::move(reg) );
}


void Epoll::addToExisting(Registrations::iterator it, Registration &&reg)
{
  assert(not it->second.empty());
  auto const fd = it->first;

  epoll_event ev;
  bzero(&ev, sizeof(ev));
  ev.data.fd = fd;
  ev.events = reg.events_;
  for(auto& r: it->second)
    ev.events |= r.events_;
  if( syscallRetry( [&]() { return epoll_ctl(epFd_.get(), EPOLL_CTL_MOD, fd, &ev); } ) == -1 )
    BUT_THROW(EpollError, "epoll_ctr(EPOLL_CTL_MOD): failed to modify fd=" << fd << " with events=" << reg.events_ << ": " << strerror(errno));
  // add at the begining, to avoid re-processing these in the very same iteration, when adding from an onEvent action for the same FD
  it->second.push_front( std::move(reg) );
}


size_t Epoll::waitImpl(int timeoutMs)
{
  auto constexpr maxEvents = 1024;
  epoll_event events[maxEvents];
  auto const n = syscallRetry( [&]() { return epoll_wait(epFd_.get(), events, maxEvents, timeoutMs); } );
  if( n == -1 )
    BUT_THROW(EpollError, "epoll_wait(): " << strerror(errno));

  // note that exception here is not an issue, as events are level-triggered, not edge-triggered.
  // after an error it's enough to re-call waitImpl().
  size_t calls = 0;
  for(auto i=0; i<n; ++i)
    calls += dispatch(events[i]);
  return calls;
}


size_t Epoll::dispatch(epoll_event const& ev)
{
  auto it = registrations_.find(ev.data.fd);
  assert( it != end(registrations_) && "FDs registrations set is inconcistent" );
  size_t calls = 0;
  for(auto& reg: it->second)
    calls += dispatch(reg, ev);
  return calls;
}


namespace
{
constexpr auto allEventTypes()
{
  return std::array{
    Epoll::Event::In,
    Epoll::Event::Pri,
    Epoll::Event::Out,
    Epoll::Event::Rdnorm,
    Epoll::Event::Rdband,
    Epoll::Event::Wrnorm,
    Epoll::Event::Wrband,
    Epoll::Event::Msg,
    Epoll::Event::Err,
    Epoll::Event::Hup,
    Epoll::Event::Rdhup,
    Epoll::Event::Exclusive,
    Epoll::Event::Wakeup,
    Epoll::Event::Oneshot,
    Epoll::Event::Et,
  };
}
}

size_t Epoll::dispatch(Registration& reg, epoll_event const& ev)
{
  if( (reg.events_ & ev.events) == 0 )
    return 0;

  size_t calls = 0;
  for(auto type: allEventTypes())
    if(ev.events & type)
      if(reg.events_ & type)
      {
        assert(reg.onEvent_);
        reg.onEvent_(ev.data.fd, type);
        if( ev.data.fd != interruptSource_.get().d2_.get() )
          ++calls;
      }
  return calls;
}

}
