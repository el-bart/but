#pragma once
#include <list>
#include <chrono>
#include <memory>
#include <unordered_map>
#include <functional>
#include <type_traits>
#include <sys/epoll.h>
#include <But/Exception.hpp>
#include <But/System/Descriptor.hpp>
#include <But/System/SocketPair.hpp>

namespace But::System
{

class Epoll final
{
public:
  BUT_DEFINE_EXCEPTION(EpollError, Exception, "But::System::Epoll failed");

  enum Event: std::underlying_type_t<EPOLL_EVENTS>
  {
    In        = EPOLLIN,
    Pri       = EPOLLPRI,
    Out       = EPOLLOUT,
    Rdnorm    = EPOLLRDNORM,
    Rdband    = EPOLLRDBAND,
    Wrnorm    = EPOLLWRNORM,
    Wrband    = EPOLLWRBAND,
    Msg       = EPOLLMSG,
    Err       = EPOLLERR,
    Hup       = EPOLLHUP,
    Rdhup     = EPOLLRDHUP,
    Exclusive = EPOLLEXCLUSIVE,
    Wakeup    = EPOLLWAKEUP,
    Oneshot   = EPOLLONESHOT,
    Et        = EPOLLET,
  };

  using OnEvent = std::function<void(int, Event)>;

  Epoll();

  Epoll(Epoll const&) = delete;
  Epoll& operator=(Epoll const&) = delete;

  Epoll(Epoll&& other) = default;
  Epoll& operator=(Epoll&& other) = default;

  void swap(Epoll& other);

  /** non-blocking wait for events. if there are no events - returns immediately.
   *  @returns number of notifications sent.
   */
  size_t check() { return waitImpl(0); }

  /** blocking wait for events. blocks w/o a timeout.
   *  @returns number of notifications sent.
   */
  size_t wait() { return waitImpl(-1); }

  /** blocking wait for events.
   *  @returns number of notifications sent.
   */
  size_t wait(std::chrono::milliseconds const timeout)
  {
    if(timeout < std::chrono::milliseconds::zero())
      BUT_THROW(EpollError, "Epoll::wait(): timeout cannot be negative");
    return waitImpl( static_cast<int>( timeout.count() ) );
  }

  /** adds new event handler, for a given FD and event(s) type(s).
   *  @note it can safely be called from onEvent().
   */
  template<typename ...Events>
  void add(int fd, OnEvent onEvent, Events ...events)
  {
    static_assert( sizeof...(events) > 0u, "some events must be specified for FD" );
    static_assert( ( std::is_same_v<Event, Events> && ... ), "all variadic args must be of Event type" );
    add( fd, Registration{ std::move(onEvent), ( static_cast<std::underlying_type_t<Event>>(events) | ... ) } );
  }

  /** removes all actions associated with a given FD.
   *  @note it can safely be called from onEvent()
   *  @warning when called from onEvent() on FD that's currently being processed, all pending events for this FD will still be called, while
   *           newly regsitered ones will not be called untill next check() / wait() call.
   */
  void remove(int fd);

  /** @brief interrupts check()/wait() calls from a separate thread.
   *  @note this call is both thread-safe and reentrant.
   *  @note once interrup() is called, it will affect the next call to check() / wait(), even if it was called after the interrup() returned. only 1 call will be affected, though.
   */
  void interrupt();

private:
  struct Registration
  {
    OnEvent onEvent_;
    std::underlying_type_t<Event> events_{};
  };
  // list<> is used as it can be expanded on the fly
  // shared_ptr<> is there to ensure remove-of-self works from onEvent() actions
  using Registrations = std::unordered_map<int, std::shared_ptr<std::list<Registration>>>;

  void add(int fd, Registration &&reg);
  void addNew(int fd, Registration &&reg);
  void addToExisting(Registrations::iterator it, Registration &&reg);
  size_t waitImpl(int timeoutMs);
  size_t dispatch(epoll_event const& ev);
  size_t dispatch(Registration& reg, epoll_event const& ev);

  SocketPair interruptSource_;
  Descriptor epFd_;
  Registrations registrations_;
};


inline void swap(Epoll& e1, Epoll& e2)
{
  e1.swap(e2);
}

}
