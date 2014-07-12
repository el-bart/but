#pragma once

#include <utility>
#include <cassert>
#include "Exception.hpp"
#include "detail/getPointerValue.hpp"

namespace But
{

template<typename P>
class NotNull
{
public:
  BUT_DEFINE_EXCEPTION(NullPointer, Exception, "unexpected nullptr received");

  explicit NotNull(P p):
    p_{ std::move(p) }
  {
    if(not p_)
      BUT_THROW(NullPointer, "constructor's' argument");
  }

  /*
  template<typename U>
  explicit NotNull(U u):
    p_{ std::move(u) }
  {
    if(not p_)
      BUT_THROW(NullPointer, "converting constructor's' argument");
  }
  */

  template<typename U>
  explicit NotNull(NotNull<U> u):
    p_{ std::move(u.p_) }
  {
    if(not p_)
      BUT_THROW(NullPointer, "converting constructor's' argument");
  }

  auto get() const { assert(p_); return detail::getPointerValue(p_); }
  auto operator->() const { return get(); }

private:
  template<typename U> friend class NotNull;

  P p_;
};

}
