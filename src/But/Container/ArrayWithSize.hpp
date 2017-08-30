#pragma once
#include <algorithm>
#include <initializer_list>
#include "But/assert.hpp"
#include "But/Mpl/SizeTypeFor.hpp"
#include "But/Container/Array.hpp"

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
  using Container = Array<T,N>;

public:
  using difference_type = typename Container::difference_type;
  using size_type = typename Mpl::SizeTypeFor<N>::type;
  using value_type = typename Container::value_type;
  using iterator = typename Container::iterator;
  using const_iterator = typename Container::const_iterator;

  constexpr ArrayWithSize() = default;
  constexpr ArrayWithSize(std::initializer_list<T> lst):
    c_{ makeFilledByMove( lst.begin(), lst.end() ) },
    size_( lst.size() )
  {
    BUT_ASSERT( lst.size() <= N && "too many arguments for a given type" );
  }

  constexpr ArrayWithSize(ArrayWithSize const& other):
    c_{ makeFilledByCopy( other.begin(), other.end() ) },
    size_( other.size() )
  { }
  constexpr ArrayWithSize& operator=(ArrayWithSize const& other)
  {
    copyOrMove(other);
    return *this;
  }

  constexpr ArrayWithSize(ArrayWithSize&& other):
    c_{ makeFilledByMove( other.begin(), other.end() ) },
    size_( other.size() )
  {
    other.size_ = 0;
  }
  constexpr ArrayWithSize& operator=(ArrayWithSize&& other)
  {
    if( copyOrMove( std::move(other) ) )
      other.size_ = 0u;
    return *this;
  }

  constexpr bool empty() const { return size() == 0u; }
  constexpr size_type size() const { return size_; }
  constexpr size_t max_size() const { return N; }

  template<typename ...Args>
  constexpr void emplace_back(Args&&... args)
  {
    push_back( value_type{ std::forward<Args>(args)... } );
  }

  constexpr void push_back(value_type const& vt)
  {
    BUT_ASSERT( size() < max_size() && "overflow detected" );
    c_[size_] = vt;
    ++size_;
  }
  constexpr void push_back(value_type&& vt)
  {
    BUT_ASSERT( size() < max_size() && "overflow detected" );
    c_[size_] = std::move(vt);
    ++size_;
  }

  constexpr void pop_back()
  {
    BUT_ASSERT( not empty() );
    --size_;
    c_[size_] = value_type{};
  }

  constexpr value_type const& operator[](const size_type pos) const
  {
    BUT_ASSERT( pos < size() && "index out of bound" );
    return c_[pos];
  }
  constexpr value_type& operator[](const size_type pos)
  {
    BUT_ASSERT( pos < size() && "index out of bound" );
    return c_[pos];
  }

  constexpr void clear()
  {
    while( not empty() )
      pop_back();
  }

  constexpr const_iterator cbegin() const { using std::begin; return begin(c_); }
  constexpr const_iterator cend() const { return cbegin() + size_; }

  constexpr const_iterator begin() const { return cbegin(); }
  constexpr const_iterator end() const { return begin() + size_; }

  constexpr iterator begin() { using std::begin; return begin(c_); }
  constexpr iterator end() { return begin() + size_; }

private:
  template<typename Other>
  constexpr bool copyOrMove(Other&& other)
  {
    if(this==&other)
      return false;
    clear();
    for(auto&& e: other)
      push_back( std::move(e) );    // TODO: move vs. forward...
    return true;
  }

  template<typename It>
  inline constexpr Container makeFilledByCopy(It begin, It end)
  {
    // helper to workaround clang bug: 
    Container c{};
    auto i = size_type{0};
    for(auto it=begin; it!=end; ++it)
      c[i++] = *it;
    return c;
  }

  template<typename It>
  inline constexpr Container makeFilledByMove(It begin, It end)
  {
    // helper to workaround clang bug: 
    Container c{};
    auto i = size_type{0};
    for(auto it=begin; it!=end; ++it)
      c[i++] = std::move(*it);
    return c;
  }

  Container c_{};
  size_type size_{0};
};


template<typename T, size_t N>
bool operator==(ArrayWithSize<T,N> const& lhs, ArrayWithSize<T,N> const& rhs)
{
  using std::begin;
  using std::end;
  return std::equal( begin(lhs), end(lhs), begin(rhs), end(rhs) );
}

template<typename T, size_t N>
bool operator!=(ArrayWithSize<T,N> const& lhs, ArrayWithSize<T,N> const& rhs)
{
  return not ( lhs == rhs );
}

template<typename T, size_t N>
bool operator<(ArrayWithSize<T,N> const& lhs, ArrayWithSize<T,N> const& rhs)
{
  return std::lexicographical_compare( lhs.begin(), lhs.end(), rhs.begin(), rhs.end() );
}

template<typename T, size_t N>
bool operator<=(ArrayWithSize<T,N> const& lhs, ArrayWithSize<T,N> const& rhs)
{
  return lhs == rhs || lhs < rhs;
}

template<typename T, size_t N>
bool operator>(ArrayWithSize<T,N> const& lhs, ArrayWithSize<T,N> const& rhs)
{
  return not ( lhs <= rhs );
}

template<typename T, size_t N>
bool operator>=(ArrayWithSize<T,N> const& lhs, ArrayWithSize<T,N> const& rhs)
{
  return not ( lhs < rhs );
}

}
}
