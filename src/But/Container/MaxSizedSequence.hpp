#pragma once
#include <vector>
#include <type_traits>
#include <cassert>

namespace But
{
namespace Container
{

template<typename T, size_t N, typename Container = std::vector<T>>
class MaxSizedSequence final
{
public:
  using size_type  = typename Container::size_type;
  using value_type = typename Container::value_type;

  MaxSizedSequence() = default;

  bool empty() const { return c_.empty(); }
  size_type size() const { return c_.size(); }
  constexpr size_t max_size() const { return N; }

  bool push_back(value_type vt)
  {
    if( c_.size() == N )
      return false;
    //c_[
    c_.push_back( std::move(vt) );
    return true;
  }

  void pop_back()
  {
    assert( not empty() );
    c_.pop_back();
  }



private:
  static_assert( std::is_same<T, value_type>::value, "container's type does not match declared" );

  Container c_;
  size_type size_{0};
  //size_type additions_;
};

}
}
