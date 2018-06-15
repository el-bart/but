#pragma once
#include <boost/optional.hpp>
#include <But/assert.hpp>

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
  Optional(T const& t): t_{t} { }
  Optional(T&& t): t_{ std::move(t) } { }

  template<typename... Args>
  explicit Optional(Args&&... args): t_{ T{ std::forward<Args>(args)... } } { }

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

  template<typename ...Args>
  void emplace(Args&&... args)
  {
    t_.emplace( std::forward<Args>(args)... );
  }

  explicit operator bool() const { return static_cast<bool>(t_); }

  T&& get() &&
  {
    BUT_ASSERT( static_cast<bool>(t_) && "optional is not set" );
    return std::move( t_.get() );
  }
  T& get() &
  {
    BUT_ASSERT( static_cast<bool>(t_) && "optional is not set" );
    return t_.get();
  }
  T const& get() const &
  {
    BUT_ASSERT( static_cast<bool>(t_) && "optional is not set" );
    return t_.get();
  }

  auto& operator*() { return get(); }
  auto& operator*() const { return get(); }

  auto operator->() { return &get(); }
  auto operator->() const { return &get(); }

  auto operator==(Optional const& rhs) const { return t_ == rhs.t_; }
  auto operator!=(Optional const& rhs) const { return t_ != rhs.t_; }
  auto operator< (Optional const& rhs) const { return t_ <  rhs.t_; }
  auto operator> (Optional const& rhs) const { return t_ >  rhs.t_; }
  auto operator<=(Optional const& rhs) const { return t_ <= rhs.t_; }
  auto operator>=(Optional const& rhs) const { return t_ >= rhs.t_; }

  void reset() { t_.reset(); }

private:
  boost::optional<T> t_;
};


template<typename T, typename ...Args>
auto makeOptional(Args&& ...args) { return Optional<T>{ std::forward<Args>(args)... }; }

}
