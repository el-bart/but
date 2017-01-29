#pragma once
#include <algorithm>

/** @brief helper macro to define all comparison operators, for a given class, based on a provided extract operator.
 *  <code>
 *  struct MyClass { std::string a_; }
 *  BUT_MPL_FREE_OPERATORS_COMPARE(MyClass, .a_)
 *  // ...
 *  const MyClass x{"foo"};
 *  const MyClass y{"bar"};
 *  assert( x > y );
 *  </code>
 */
#define BUT_MPL_FREE_OPERATORS_COMPARE(Class, Extract) \
  inline bool operator< (Class const& lhs, Class const& rhs) { return lhs Extract <  rhs Extract ; } \
  inline bool operator<=(Class const& lhs, Class const& rhs) { return lhs Extract <= rhs Extract ; } \
  inline bool operator> (Class const& lhs, Class const& rhs) { return lhs Extract >  rhs Extract ; } \
  inline bool operator>=(Class const& lhs, Class const& rhs) { return lhs Extract >= rhs Extract ; } \
  inline bool operator==(Class const& lhs, Class const& rhs) { return lhs Extract == rhs Extract ; } \
  inline bool operator!=(Class const& lhs, Class const& rhs) { return lhs Extract != rhs Extract ; }


/** @brief helper macro to define all comparison operators, for a given collection type
 *  <code>
 *  struct MyCollection
 *  {
 *    auto begin() { ... }
 *    auto end() { ... }
 *  }
 *  BUT_MPL_FREE_OPERATORS_COLLECTION_COMPARE(MyCollection)
 *  // ...
 *  const MyCollection c1{...};
 *  const MyCollection c2{...};
 *  assert( c1 > c2 );
 *  </code>
 */
#define BUT_MPL_FREE_OPERATORS_COLLECTION_COMPARE(Collection) \
  bool operator==(Collection const& lhs, Collection const& rhs) \
  { \
    using std::begin; \
    using std::end; \
    return std::equal( begin(lhs), end(lhs), begin(rhs), end(rhs) ); \
  } \
  bool operator!=(Collection const& lhs, Collection const& rhs) \
  { \
    return not ( lhs == rhs ); \
  } \
  bool operator<(Collection const& lhs, Collection const& rhs) \
  { \
    return std::lexicographical_compare( lhs.begin(), lhs.end(), rhs.begin(), rhs.end() ); \
  } \
  bool operator<=(Collection const& lhs, Collection const& rhs) \
  { \
    return lhs == rhs || lhs < rhs; \
  } \
  bool operator>(Collection const& lhs, Collection const& rhs) \
  { \
    return not ( lhs <= rhs ); \
  } \
  bool operator>=(Collection const& lhs, Collection const& rhs) \
  { \
    return not ( lhs < rhs ); \
  }
