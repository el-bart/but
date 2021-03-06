#pragma once
#include <utility>
#include <type_traits>
#include <boost/operators.hpp>

namespace But
{
namespace Mpl
{

#define BUT_MPL_VALUE_WRAP(name, type) \
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
    name(name const&) = default; \
    name& operator=(name const&) = default; \
    \
    name(name&&) = default; \
    name& operator=(name&&) = default; \
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
}
