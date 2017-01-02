#pragma once
#include <vector>
#include <iterator>
#include <initializer_list>
#include <cassert>

namespace But
{
namespace Container
{

/** @brief
 *  cache-friendly data structure, meant to have fast insertion (O(1)), removal (O(1)) and iteration.
 *
 *  @par
 *  structure is essentially an array, but with operations defined in a special way, so that its modification
 *  is fast. arrays are cache-friendly, so it is fast on typical operations. to make modifications fast,
 *  no particualr roder is guaranteed.
 *
 *  @par
 *  search is always linear (O(N)), though typically it works blazing fast on modern hardware.
 *
 *  @par
 *  container provides random-access iterators to elements.
 *
 *  @par
 *  structure does not guaranee any particular order of elements. operating on actuall elemnts
 *  (via indexes or iterators) does not change order of others. for instance container can be sorder,
 *  but after sorting any addition or removal may break this order.
 *
 *  @par
 *  each insertion or removal invalidates all iterators.
 */
template<typename T, typename Allocator = std::allocator<T>>
class UnorderedArray final
{
  using ImplContainer = std::vector<T, Allocator>;
public:
  using value_type     = T;
  using allocator_type = typename ImplContainer::allocator_type;
  using size_type      = typename ImplContainer::size_type;
  using iterator       = typename ImplContainer::iterator;
  using const_iterator = typename ImplContainer::const_iterator;

  UnorderedArray() = default;
  UnorderedArray(std::initializer_list<T> const& lst): c_{lst} { }

  bool empty() const { return c_.empty(); }
  size_type size() const { return c_.size(); }
  size_type capacity() const { return c_.capacity(); }
  void reserve(size_type space) { c_.reserve(space); }

  void add(T&& t) { c_.push_back( std::move(t) ); }
  void add(T const& t) { c_.push_back(t); }

  template<typename ...Args>
  void emplace(Args&& ...args) { c_.emplace_back( std::forward<Args>(args)... ); }

  iterator begin() { return std::begin(c_); }
  const_iterator begin() const { return std::begin(c_); }
  iterator end() { return std::end(c_); }
  const_iterator end() const { return std::end(c_); }
  const_iterator cbegin() const { return begin(); }
  const_iterator cend() const { return end(); }

  value_type& operator[](const size_type pos)
  {
    assert( pos < size() && "index out of range" );
    return c_[pos];
  }
  value_type const& operator[](const size_type pos) const
  {
    assert( pos < size() && "index out of range" );
    return c_[pos];
  }

  /** @brief remove element under a given iterator from container.
   *  @par
   *  this call reorders elements to make removal faster. it can also
   *  decrease capacity, if there is too much space being waisted.
   */
  void erase(const_iterator it)
  {
    assert( it != end() && "cannot operate on unreferenceble iterator" );
    // if last (or the only one at all), just remove it
    if( it + 1 == end() )
    {
      c_.pop_back();
      return;
    }
    // otherwise remove by replacing element to be removed with the last one
    // and remove the last one (already-moved-from).
    assert( c_.size() > 1u );
    const auto pos = std::distance( cbegin(), it );
    c_[pos] = std::move_if_noexcept( c_.back() );
    c_.pop_back();
    decreaseCapacityIfPlausable();
  }

  void clear()
  {
    ImplContainer tmp;
    c_.swap(tmp);
  }

  void swap(UnorderedArray<T>& other) { c_.swap(other.c_); }

private:
  void decreaseCapacityIfPlausable()
  {
    // do this only for bigger sizes, to avoid relocations on small containers
    if( size() < 20u )
      return;
    // if there is over 50% of unused space, decrease container size
    if( capacity() > 2u*size() )
      c_.shrink_to_fit();
  }

  ImplContainer c_;
};


template<typename T>
void swap(UnorderedArray<T>& lhs, UnorderedArray<T>& rhs)
{
  lhs.swap(rhs);
}

}
}
