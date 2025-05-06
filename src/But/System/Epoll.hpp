#pragma once
#include <chrono>
#include <initializer_list>
#include <But/System/Descriptor.hpp>

namespace But::System
{

class Epoll final
{
public:
  struct Registration
  {
    // TODO
  };

  struct Notification
  {
    // TODO
  };
  using value_type = std::vector<Notification>;

  Epoll(): Epoll( {} ) { }
  explicit Epoll(std::initializer_list<Registration> regs)
  {
    for(auto r: regs)
      add(r);
    // TODO: interruptions
  }

  ~Epoll(); // TODO

  Epoll(Epoll const&) = delete;
  Epoll& operator=(Epoll const&) = delete;

  Epoll(Epoll&& other) = default;
  Epoll& operator=(Epoll&& other) = default;

  void swap(Epoll& other)
  {
    (void)other; // TODO
    //std::swap(other.pair_, pair_);
  }

  /** non-blocking wait for events. if there are no events - returns immediately.
   *  @returns reference to local collection of response values. it is overwritten upon each call to wait().
   */
  value_type const& check();

  /** blocking wait for events.
   *  @returns reference to local collection of response values. it is overwritten upon each call to wait().
   */
  value_type const& wait(std::chrono::milliseconds timeout = std::chrono::milliseconds{0});

  /** blocking wait for events. blocks w/o a timeout.
   *  @returns reference to local collection of response values. it is overwritten upon each call to wait().
   */
  value_type const& wait();

  void add(Registration const& reg); // TODO
  void remove(int fd); // TODO

  /** @brief interrupts check()/wait() calls from a separate thread.
   *  @note this call is both thread-safe and reentrant.
   *  @note once interrup() is called, it will affect the next call to check() / wait(), even if it was called after the interrup() returned. only 1 call will be affected, though.
   */
  void interrupt(); // TODO

private:
  value_type results;
  // TODO
};


inline void swap(Epoll& e1, Epoll& e2)
{
  e1.swap(e2);
}

}
