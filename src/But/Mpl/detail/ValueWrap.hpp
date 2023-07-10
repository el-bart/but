#pragma once
#include <utility>
#include <type_traits>
#include <initializer_list>
#include <boost/operators.hpp>

namespace But::Mpl::detail
{

#define BUT_MPL_DETAIL_VALUE_WRAP_COPYABLE(name) \
    name(name&) = default; \
    name(name const&) = default; \
    name& operator=(name const&) = default;

#define BUT_MPL_DETAIL_VALUE_WRAP_NON_COPYABLE(name) \
    name(name const&) = delete; \
    name& operator=(name const&) = delete;


#define BUT_MPL_DETAIL_VALUE_WRAP_MOVABLE(name) \
    name(name&&) = default; \
    name& operator=(name&&) = default;

#define BUT_MPL_DETAIL_VALUE_WRAP_NON_MOVABLE(name) \
    name(name&&) = delete; \
    name& operator=(name&&) = delete;


#define BUT_MPL_DETAIL_VALUE_WRAP(name, COPYABILITY, MOVABILITY, type) \
  class name final: public boost::equivalent<name>, \
                    public boost::totally_ordered<name> \
  { \
  public: \
    template<typename ...Args> \
    explicit name(Args&& ...args): \
      /* NOTE: value_(...) not value_{...} - see initializer_list<> overload */ \
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
    COPYABILITY(name) \
    MOVABILITY(name) \
    \
    auto operator<(name const& rhs) const { return get() < rhs.get(); } \
    \
    type const& get() const & { return value_; } \
    type&& get() && { return std::move(value_); } \
    \
  private: \
    type value_; \
  }

}
