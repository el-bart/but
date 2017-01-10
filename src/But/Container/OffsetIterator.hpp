#pragma once
#include <cassert>
#include <boost/iterator/iterator_facade.hpp>
#include "detail/ConstPreservingValueType.hpp"

namespace But
{
namespace Container
{

/** @brief wrapper making random access, (const) iterator for a collection.
 *         collection elements are access by the collection pointer and index.
 */
template<typename Collection>
class OffsetIterator final: public boost::iterator_facade< OffsetIterator<Collection>,
                                                           typename detail::ConstPreservingValueType<Collection>::type,
                                                           boost::bidirectional_traversal_tag
                                                         >
{
public:
  using value_type = typename detail::ConstPreservingValueType<Collection>::type;
  using size_type = typename Collection::size_type;

  /** @brief converting c-tor for iterator->const_iterator and copying.
   */
  template<typename OtherCollection>
  OffsetIterator(OffsetIterator<OtherCollection> const& other):
    c_{other.c_},
    pos_{other.pos_}
  { }

  explicit OffsetIterator(Collection& c, const size_type pos=0):
    c_{&c},
    pos_{pos}
  {
    assert( pos_ <= c_->size() && "iterator out of bound" );  // NOTE: pos == size is end()
  }

private:
  // this friend declaration is required for boost::iterators to work.
  friend class boost::iterator_core_access;

  template<typename>
  friend class OffsetIterator;

  template<typename OtherCollection>
  bool equal(OffsetIterator<OtherCollection> const& other) const
  {
    assert( c_ == other.c_ && "comparing iterators from different collections");
    return pos_ == other.pos_;
  }

  void increment()
  {
    ++pos_;
    assert( pos_ <= c_->size() );
  }

  void decrement()
  {
    assert( 0u < pos_ && "trying to decrement begin iterator - probably a bug");
    --pos_;
  }

  auto& dereference()
  {
    assert( pos_ <= c_->size() );
    return (*c_)[pos_];
  }

  auto& dereference() const
  {
    assert( pos_ <= c_->size() );
    return (*c_)[pos_];
  }

  Collection* c_;
  size_type pos_;
};

}
}
