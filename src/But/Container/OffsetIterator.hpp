#pragma once
#include "But/assert.hpp"
#include <boost/iterator/iterator_facade.hpp>
#include "detail/ConstPreservingValueType.hpp"

namespace But
{
namespace Container
{

/** @brief wrapper making random access, (const) iterator for a collection.
 *         collection elements are access by the collection pointer and index.
 */
template<typename Collection, typename Reference = typename detail::ConstPreservingValueType<Collection>::type&>
class OffsetIterator final: public boost::iterator_facade< OffsetIterator<Collection, Reference>,
                                                           typename detail::ConstPreservingValueType<Collection>::type,
                                                           boost::random_access_traversal_tag,
                                                           Reference
                                                         >
{
public:
  using value_type = typename detail::ConstPreservingValueType<Collection>::type;
  using size_type = typename Collection::size_type;
  using difference_type = typename Collection::difference_type;

  /** @brief converting c-tor for iterator->const_iterator and copying.
   */
  template<typename OtherCollection, typename OtherReference>
  OffsetIterator(OffsetIterator<OtherCollection, OtherReference> const& other):
    c_{other.c_},
    pos_{other.pos_}
  { }

  explicit OffsetIterator(Collection& c, const size_type pos=0):
    c_{&c},
    pos_{pos}
  {
    BUT_ASSERT( pos_ <= c_->size() && "iterator out of bound" );  // NOTE: pos == size is end()
  }

private:
  // this friend declaration is required for boost::iterators to work.
  friend class boost::iterator_core_access;

  template<typename, typename>
  friend class OffsetIterator;

  template<typename OtherCollection, typename OtherReference>
  bool equal(OffsetIterator<OtherCollection, OtherReference> const& other) const
  {
    BUT_ASSERT( c_ == other.c_ && "comparing iterators from different collections");
    return pos_ == other.pos_;
  }

  void increment()
  {
    ++pos_;
    BUT_ASSERT( pos_ <= c_->size() );
  }

  void decrement()
  {
    BUT_ASSERT( 0u < pos_ && "trying to decrement begin iterator - probably a bug");
    --pos_;
  }

  void advance(const difference_type diff)
  {
    pos_ += diff;
    BUT_ASSERT( pos_ <= c_->size() && "index out of range" );
  }

  Reference dereference() const
  {
    BUT_ASSERT( pos_ <= c_->size() );
    return (*c_)[pos_];
  }

  Collection* c_;
  size_type pos_;
};

}
}
