#pragma once
#include <vector>
#include <type_traits>
#include <cassert>

namespace But
{
namespace Container
{

template<typename T, size_t N, typename Container = std::vector<T>>
class OverflowingSequence final
{
public:
  using size_type = typename Container::size_type;
  using value_type = typename Container::value_type;
  using iterator = typename Container::iterator;
  using const_iterator = typename Container::const_iterator;

  bool empty() const { return c_.empty(); }
  size_type size() const { return c_.size(); }
  constexpr size_t max_size() const { return N; }

  bool push_back(value_type vt)
  {
    if( c_.size() == N )
      return false;
    c_.push_back( std::move(vt) );
    return true;
  }

  void pop_back()
  {
    assert( not empty() );
    c_.pop_back();
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
  const_iterator cend() const { using std::end; return end(c_); }

  const_iterator begin() const { return cbegin(); }
  const_iterator end() const { return cend(); }

  iterator begin() { using std::begin; return begin(c_); }
  iterator end() { using std::end; return end(c_); }



private:
  static_assert( std::is_same<T, value_type>::value, "container's type does not match declared" );

  Container c_;
  //size_type additions_;
};

}
}
