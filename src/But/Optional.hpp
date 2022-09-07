#pragma once
#include <optional>
#include <But/assert.hpp>

namespace But
{

/** @brief wrapper and a drop-in replacement for std::optional.
 *         the biggest difference from std::optional is that But::Optional becomes empty, when moved from.
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
  Optional(Optional&) = default;
  Optional& operator=(Optional&) = default;

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

  auto has_value() const { return static_cast<bool>(t_); }
  explicit operator bool() const { return has_value(); }

  T&& value() &&
  {
    BUT_ASSERT( static_cast<bool>(t_) && "optional is not set" );
    return std::move( t_.value() );
  }
  T& value() &
  {
    BUT_ASSERT( static_cast<bool>(t_) && "optional is not set" );
    return t_.value();
  }
  T const& value() const &
  {
    BUT_ASSERT( static_cast<bool>(t_) && "optional is not set" );
    return t_.value();
  }

  // TODO: add value_or()

  auto& operator*() { return value(); }
  auto& operator*() const { return value(); }

  auto operator->() { return &value(); }
  auto operator->() const { return &value(); }

  auto operator==(Optional const& rhs) const { return t_ == rhs.t_; }
  auto operator!=(Optional const& rhs) const { return t_ != rhs.t_; }
  auto operator< (Optional const& rhs) const { return t_ <  rhs.t_; }
  auto operator> (Optional const& rhs) const { return t_ >  rhs.t_; }
  auto operator<=(Optional const& rhs) const { return t_ <= rhs.t_; }
  auto operator>=(Optional const& rhs) const { return t_ >= rhs.t_; }

  void reset() { t_.reset(); }

private:
  std::optional<T> t_;
};


template<typename T, typename ...Args>
auto makeOptional(Args&& ...args) { return Optional<T>{ std::forward<Args>(args)... }; }

}
