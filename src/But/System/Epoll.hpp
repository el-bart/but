#pragma once
#include <chrono>
#include <functional>
#include <linux/eventpoll.h>
#include <But/System/Descriptor.hpp>
#include <But/System/SocketPair.hpp>

namespace But::System
{

class Epoll final
{
public:
  enum Event: decltype(EPOLLIN)
  {
    In     = EPOLLIN,
    Pri    = EPOLLPRI,
    Out    = EPOLLOUT,
    Err    = EPOLLERR,
    Hup    = EPOLLHUP,
    Nval   = EPOLLNVAL,
    Rdnorm = EPOLLRDNORM,
    Rdband = EPOLLRDBAND,
    Wrnorm = EPOLLWRNORM,
    Wrband = EPOLLWRBAND,
    Msg    = EPOLLMSG,
    Rdhup  = EPOLLRDHUP,
  };

  using OnEvent = std::function<void(int, Event)>;

  struct Registration
  {
    template<typename ...Events>
    Registration(OnEvent onEvent, int fd, Events ...events):
      onEvent_{ std::move(onEvent) },
      fd_{fd},
      events_{ ( static_cast<decltype(EPOLLIN)>(events) | ... ) }
    {
      static_assert( sizeof...(events) > 0u, "some events must be specified for FD" );
      static_assert( ( std::is_same_v<Event, Events> && ... ), "all variadic args must be of Event type" );
    }

    OnEvent onEvent_;
    int fd_{-1};
    decltype(EPOLLIN) events_{};
  };

  Epoll();
  //~Epoll(); // TODO

  Epoll(Epoll const&) = delete;
  Epoll& operator=(Epoll const&) = delete;

  Epoll(Epoll&& other) = default;
  Epoll& operator=(Epoll&& other) = default;

  void swap(Epoll& other)
  {
    (void)other; // TODO
    using std::swap;
    //std::swap(other.pair_, pair_);
  }

  /** non-blocking wait for events. if there are no events - returns immediately.
   *  @returns number of notifications sent.
   */
  size_t check();

  /** blocking wait for events.
   *  @returns number of notifications sent.
   */
  size_t wait(std::chrono::milliseconds timeout = std::chrono::milliseconds{0});

  /** blocking wait for events. blocks w/o a timeout.
   *  @returns number of notifications sent.
   */
  size_t wait();

  void add(Registration reg); // TODO
  void remove(int fd); // TODO

  /** @brief interrupts check()/wait() calls from a separate thread.
   *  @note this call is both thread-safe and reentrant.
   *  @note once interrup() is called, it will affect the next call to check() / wait(), even if it was called after the interrup() returned. only 1 call will be affected, though.
   */
  void interrupt(); // TODO

private:
  SocketPair irq;
  Descriptor epFd;
  // TODO
};


inline void swap(Epoll& e1, Epoll& e2)
{
  e1.swap(e2);
}

}
