#pragma once
#include <array>
#include <initializer_list>
#include <cassert>
#include "But/Mpl/SizeTypeFor.hpp"

namespace But
{
namespace Container
{

/** @brief simple std::array<> replacement, that keeps number of elements. this way
 *         container can appear to be resizing, even though the maximum number of elements
 *         is predefined with N.
 *
 *  @note elements T must be default-constructible, just like in regular std::array<>
 *
 *  @note when element is removed, it is internally overwritten with a default-value.
 */
template<typename T, size_t N>
class ArrayWithSize final
{
  using Container = std::array<T,N>;

public:
  using difference_type = typename Container::difference_type;
  using size_type = typename Mpl::SizeTypeFor<N>::type;
  using value_type = typename Container::value_type;
  using iterator = typename Container::iterator;
  using const_iterator = typename Container::const_iterator;

  ArrayWithSize() = default;
  ArrayWithSize(std::initializer_list<T> lst)
  {
    assert( lst.size() <= N && "too many arguments for a given type" );
    for(auto& e: lst)
      push_back(e);
  }
  ArrayWithSize(ArrayWithSize const& other) = default;
  ArrayWithSize& operator=(ArrayWithSize const& other) = default;
  ArrayWithSize(ArrayWithSize&& other) = default;
  ArrayWithSize& operator=(ArrayWithSize&& other) = default;

  bool empty() const { return size() == 0u; }
  size_type size() const { return size_; }
  constexpr size_t max_size() const { return N; }

  void push_back(value_type vt)
  {
    assert( size() < max_size() && "overflow detected" );
    c_[size_] = std::move(vt);
    ++size_;
  }

  void pop_back()
  {
    assert( not empty() );
    --size_;
    c_[size_] = value_type{};
  }

  value_type const& operator[](const size_type pos) const
  {
    assert( pos < size() && "index out of bound" );
    return c_[pos];
  }
  value_type& operator[](const size_type pos)
  {
    assert( pos < size() && "index out of bound" );
    return c_[pos];
  }

  const_iterator cbegin() const { using std::begin; return begin(c_); }
  const_iterator cend() const { return cbegin() + size_; }

  const_iterator begin() const { return cbegin(); }
  const_iterator end() const { return begin() + size_; }

  iterator begin() { using std::begin; return begin(c_); }
  iterator end() { return begin() + size_; }

private:
  Container c_;
  size_type size_{0};
};

}
}
