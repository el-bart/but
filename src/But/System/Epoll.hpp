#pragma once
#include <chrono>
#include <vector>
#include <unordered_map>
#include <functional>
#include <type_traits>
#include <sys/epoll.h>
#include <But/System/Descriptor.hpp>
#include <But/System/SocketPair.hpp>

namespace But::System
{

class Epoll final
{
public:
  enum Event: uint32_t
  {
    In     = EPOLLIN,
    Pri    = EPOLLPRI,
    Out    = EPOLLOUT,
    Err    = EPOLLERR,
    Hup    = EPOLLHUP,
    RdNorm = EPOLLRDNORM,
    RdBand = EPOLLRDBAND,
    WrNorm = EPOLLWRNORM,
    WrBand = EPOLLWRBAND,
    Msg    = EPOLLMSG,
    RdHup  = EPOLLRDHUP,
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
  size_t check();

  /** blocking wait for events. blocks w/o a timeout.
   *  @returns number of notifications sent.
   */
  size_t wait();

  /** blocking wait for events.
   *  @returns number of notifications sent.
   */
  size_t wait(std::chrono::milliseconds timeout);

  template<typename ...Events>
  void add(int fd, OnEvent onEvent, Events ...events)
  {
    static_assert( sizeof...(events) > 0u, "some events must be specified for FD" );
    static_assert( ( std::is_same_v<Event, Events> && ... ), "all variadic args must be of Event type" );
    add( fd, Registration{ std::move(onEvent), ( static_cast<decltype(EPOLLIN)>(events) | ... ) } );
  }

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
    decltype(EPOLLIN) events_{};
  };

  void add(int fd, Registration &&reg);

  SocketPair irq_;
  Descriptor epFd_;
  std::unordered_map<int, std::vector<Registration>> registrations_;
};


inline void swap(Epoll& e1, Epoll& e2)
{
  e1.swap(e2);
}

}
