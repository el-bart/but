#pragma once
#include <memory>
#include <utility>
#include "But/assert.hpp"
#include <boost/operators.hpp>
#include "Exception.hpp"
#include "detail/getPointerValue.hpp"
#include "detail/TypesResolver.hpp"

namespace But
{

template<typename P>
class NotNull final: public boost::equivalent<NotNull<P>>,
                     public boost::totally_ordered<NotNull<P>>
{
public:
  BUT_DEFINE_EXCEPTION(NullPointer, Exception, "unexpected nullptr received");

  using element_type = typename detail::TypesResolver<P>::element_type;

  template<typename U>
  explicit NotNull(U u):
    p_{ std::move(u) }
  {
    if(not p_)
      BUT_THROW(NullPointer, "constructor's' argument");
  }

  template<typename U>
  NotNull(NotNull<U> const& u):
    p_{u.p_}
  {
    assert(p_);
  }

  template<typename U>
  NotNull(NotNull<U>&& u):
    p_{ std::move(u.p_) }
  {
    assert(p_);
  }

  NotNull<P>& operator=(P p)
  {
    // NOTE: this is not a template on purpose - constructions such as
    //       NotNull<shared_ptr<X>> x{...}; x = new X;
    //       should never compile, exactly the same way they do not for smart pointers.
    if(not p)
      BUT_THROW(NullPointer, "assignment operator");
    p_ = std::move(p);
    return *this;
  }

  template<typename U>
  NotNull<P>& operator=(NotNull<U> const& other)
  {
    p_ = other.p_;
    assert(p_);
    return *this;
  }

  template<typename U>
  NotNull<P>& operator=(NotNull<U>&& other)
  {
    p_ = std::move(other.p_);
    assert(p_);
    return *this;
  }

  auto get() const { assert(p_); return detail::getPointerValue(p_); }
#if 0
  auto operator->() const { return get(); }
#else
  // TODO: temporary workaround for GCC bug (https://gcc.gnu.org/bugzilla/show_bug.cgi?id=81182)
  element_type* operator->() const { return get(); }
#endif
  P underlyingPointer() const & { assert(p_); return p_; }
  P underlyingPointer() &&      { assert(p_); return std::move(p_); }
  [[deprecated]] P pointer() const & { return underlyingPointer(); }
  [[deprecated]] P pointer() &&      { return underlyingPointer(); }
  auto& operator*() const { return *get(); }

  explicit operator bool() const { assert(p_); return true; }

  bool operator<(NotNull<P> const& other) const { return get() < other.get(); }

private:
  template<typename U> friend class NotNull;

  P p_;
};



template<typename P>
auto makeNN(P p)
{
  return NotNull<P>{ std::move(p) };
}

template<typename P, typename ...Args>
auto makeUniqueNN(Args&&... args)
{
  return makeNN( std::make_unique<P>( std::forward<Args>(args)... ) );
}

template<typename P, typename ...Args>
auto makeSharedNN(Args&&... args)
{
  return makeNN( std::make_shared<P>( std::forward<Args>(args)... ) );
}


template<typename P>
using NotNullRaw = NotNull<P*>;

template<typename P>
using NotNullUnique = NotNull<std::unique_ptr<P>>;

template<typename P>
using NotNullShared = NotNull<std::shared_ptr<P>>;

}
