#pragma once
#include <utility>
#include <type_traits>

namespace But
{

#define BUT_VALUE_WRAP(name, type) \
  class name final \
  { \
  public: \
    template<typename ...Args> \
    explicit name(Args&& ...args): \
      value_( std::forward<Args>(args)... ) \
    { } \
    \
    template<typename T, \
             typename = std::enable_if_t< std::is_constructible<type, std::initializer_list<T>>::value > \
            > \
    name(std::initializer_list<T> lst): \
      value_{lst} \
    { } \
    \
    name(name const&) = default; \
    name& operator=(name const&) = default; \
    \
    name(name&&) = default; \
    name& operator=(name&&) = default; \
    \
    type const& get() const & { return value_; } \
    type&& get() && { return std::move(value_); } \
    \
  private: \
    type value_; \
  }

}
