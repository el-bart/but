#pragma once
#include <boost/optional.hpp>

namespace But
{

/** @brief wrapper for boost::optional, that does not behave well in many releases of boost, namely:
 *         move-construction does not work, reference types can be accepted, bool() conversion is not explicit.
 *         these are mostly historical concenrs, but stil valid on older releases at least...
 */
template<typename T>
class Optional
{
  static_assert( not std::is_reference<T>::value, "references are not allowed" );

public:
  using value_type = T;

  Optional() = default;
  explicit Optional(T&& t): t_{ std::forward<T>(t) } { }

  Optional(Optional const&) = default;
  Optional& operator=(Optional const&) = default;

  Optional(Optional&& other):
    t_{ std::move(other.t_) }
  {
    other.reset();
  }
  Optional& operator=(Optional&& other)
  {
    if( this != &other )
    {
      t_ = std::move(other.t_);
      other.reset();
    }
    return *this;
  }

  explicit operator bool() const { return static_cast<bool>(t_); }

  T& get()
  {
    assert( static_cast<bool>(t_) && "optional is not set" );
    return t_.get();
  }
  T const& get() const
  {
    assert( static_cast<bool>(t_) && "optional is not set" );
    return t_.get();
  }

  auto& operator*() { return get(); }
  auto& operator*() const { return get(); }

  auto operator->() { return &get(); }
  auto operator->() const { return &get(); }

  void reset() { t_.reset(); }

private:
  boost::optional<T> t_;
};


template<typename T, typename ...Args>
auto makeOptional(Args&& ...args) { return Optional<T>{ std::forward<Args>(args)... }; }

}
