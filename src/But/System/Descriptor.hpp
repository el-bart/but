#pragma once
#include <unistd.h>
#include <utility>

namespace But
{
namespace System
{

class Descriptor final
{
public:
  using value_type = int;

  Descriptor(): Descriptor(-1) { } // needed where explicit c-tor cannot be easily used
  explicit Descriptor(const value_type desc): desc_{desc} { }
  ~Descriptor() { reset(); }

  Descriptor(Descriptor const&) = delete;
  Descriptor& operator=(Descriptor const&) = delete;

  Descriptor(Descriptor&& other): desc_{ other.release() } { }
  Descriptor& operator=(Descriptor&& other)
  {
    Descriptor tmp{ std::move(other) };
    swap(tmp);
    return *this;
  }

  explicit operator bool() const { return opened(); }

  value_type get() const { return desc_; }

  value_type release()
  {
    const auto tmp = desc_;
    desc_ = -1;
    return tmp;
  }

  void reset(const value_type desc = -1)
  {
    if( opened() )
      close(desc_);
    desc_ = desc;
  }

  bool opened() const { return get() != -1; }

  void swap(Descriptor& other)
  {
    std::swap(other.desc_, desc_);
  }

private:
  value_type desc_;
};


inline void swap(Descriptor& d1, Descriptor& d2)
{
  d1.swap(d2);
}

}
}
