#pragma once
#include <But/Mpl/SizeTypeFor.hpp>

namespace But
{
namespace Container
{

/** @brief std::array<> replacement, until C++17 arrives with constexpr std::array<> implementation.
 *  @note implementation is very simplistic, as it is not expected to leave long... ;)
 */
template<typename T, size_t N>
struct Array
{
  using difference_type = std::ptrdiff_t;
  using size_type = typename Mpl::SizeTypeFor<N>::type;
  using value_type = T;
  using iterator = T*;
  using const_iterator = T const*;

  constexpr auto const& operator[](size_type i) const { return value_[i]; }
  constexpr auto& operator[](size_type i) { return value_[i]; }

  constexpr size_type size() const { return N; }
  constexpr size_type max_size() const { return N; }

  constexpr auto begin() const { return value_; }
  constexpr auto end() const { return begin() + N; }

  constexpr auto begin() { return value_; }
  constexpr auto end() { return begin() + N; }

  constexpr void fill(T const& t)
  {
    for(auto& e: *this)
      e = t;
  }

  T value_[N]{};
};


template<typename T, size_t N>
constexpr auto begin(Array<T,N>& tab) { return tab.begin(); }
template<typename T, size_t N>
constexpr auto end(Array<T,N>& tab) { return tab.end(); }

template<typename T, size_t N>
constexpr auto begin(Array<T,N> const& tab) { return tab.begin(); }
template<typename T, size_t N>
constexpr auto end(Array<T,N> const& tab) { return tab.end(); }

template<typename T, size_t N>
constexpr auto cbegin(Array<T,N> const& tab) { return tab.begin(); }
template<typename T, size_t N>
constexpr auto cend(Array<T,N> const& tab) { return tab.end(); }

}
}
