#pragma once
#include <utility>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <But/Exception.hpp>
#include <But/System/Descriptor.hpp>

namespace But::System
{

class SocketPair final
{
public:
  BUT_DEFINE_EXCEPTION(CannotCreateSocketPair, Exception, "socketpair()");

  struct Pair
  {
    Descriptor d1_;
    Descriptor d2_;
  };

  using value_type = Pair;

  SocketPair(): pair_{ init() } { }
  ~SocketPair() = default;

  SocketPair(SocketPair const&) = delete;
  SocketPair& operator=(SocketPair const&) = delete;

  SocketPair(SocketPair&& other) = default;
  SocketPair& operator=(SocketPair&& other) = default;

  explicit operator bool() const { return opened(); }

  value_type const& get() const { return pair_; }

  value_type release()
  {
    value_type tmp;
    using std::swap;
    swap(tmp, pair_);
    return tmp;
  }

  void reset(value_type pair = Pair{})
  {
    pair_ = std::move(pair);
  }

  bool opened() const { return pair_.d1_.opened() && pair_.d2_.opened(); }

  void swap(SocketPair& other)
  {
    std::swap(other.pair_, pair_);
  }

private:
  static value_type init()
  {
    int sp[2];
    if( syscallRetry( [&]() { return socketpair(AF_UNIX, SOCK_STREAM, 0, sp); } ) == -1 )
      BUT_THROW(CannotCreateSocketPair, "syscall failed: " << strerror(errno));
    return Pair{ Descriptor{sp[0]}, Descriptor{sp[1]} };
  }

  value_type pair_;
};


inline void swap(SocketPair& d1, SocketPair& d2)
{
  d1.swap(d2);
}

}
