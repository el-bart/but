#pragma once

#include <utility>
#include <cassert>
#include <boost/operators.hpp>
#include "Exception.hpp"
#include "detail/getPointerValue.hpp"

namespace But
{

template<typename P>
class NotNull: public boost::equivalent<NotNull<P>>,
               public boost::totally_ordered<NotNull<P>>
{
public:
  BUT_DEFINE_EXCEPTION(NullPointer, Exception, "unexpected nullptr received");

  explicit NotNull(P p):
    p_{ std::move(p) }
  {
    if(not p_)
      BUT_THROW(NullPointer, "constructor's' argument");
  }

  template<typename U>
  explicit NotNull(NotNull<U> u):
    p_{ std::move(u.p_) }
  {
    assert(p_);
  }

  template<typename U>
  explicit NotNull(U u):
    p_{ std::move(u) }
  {
    if(not p_)
      BUT_THROW(NullPointer, "converting constructor's' argument");
  }

  auto get() const { assert(p_); return detail::getPointerValue(p_); }
  auto operator->() const { return get(); }
  P pointer() const  { assert(p_); return p_; }
  P pointerMove() && { assert(p_); return std::move(p_); }

  explicit operator bool() const { assert(p_); return true; }

  bool operator<(NotNull<P> const& other) const { return get() < other.get(); }

private:
  template<typename U> friend class NotNull;

  P p_;
};

// TODO: typedefs
// TODO: copy assignment
// TODO: move assignment

}
