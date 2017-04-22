#pragma once

namespace But
{
namespace Container
{

/** @brief std::array<> replacement, until C++17 arrives with constexpr std::array<> implementation.
 *  @note implementation is very simplistic, as it is not expected to leave long... ;)
 */
template<typename T, unsigned N>
struct Array
{
  constexpr auto const& operator[](unsigned i) const { return value_[i]; }
  constexpr auto& operator[](unsigned i) { return value_[i]; }

  constexpr auto size() const { return N; }

  constexpr auto begin() const { return value_; }
  constexpr auto end() const { return begin() + N; }

  constexpr auto begin() { return value_; }
  constexpr auto end() { return begin() + N; }

  constexpr void fill(T const& t)
  {
    for(auto& e: *this)
      e = t;
  }

  T value_[N];
};

}
}
