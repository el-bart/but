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


/** @brief helper macro to define all comparison operators, for a given template class, based on a provided extract operator.
 *  <code>
 *  template<typename T>
 *  struct MyClass { T a_; }
 *  BUT_MPL_FREE_OPERATORS_COMPARE(MyClass<T>, .a_, typename T)
 *  // ...
 *  const MyClass<std::string> x{"foo"};
 *  const MyClass<std::string> y{"bar"};
 *  assert( x > y );
 *  </code>
 *
 *  @note for class with multiple template parameters, helper macro definition is needed:
 *  <code>
 *  template<typename T, typename U>
 *  struct MyClass { T a_; }
 *  #define CLASS MyClass<T,U>
 *  BUT_MPL_FREE_OPERATORS_COMPARE(CLASS, .a_, typename T, typename U)
 *  #ifdef CLASS
 *  // ...
 *  const MyClass<std::string,int> x{"foo"};
 *  const MyClass<std::string,int> y{"bar"};
 *  assert( x > y );
 *  </code>
 */
#define BUT_MPL_FREE_OPERATORS_TEMPLATE_COMPARE(Class, Extract, ...) \
  template<__VA_ARGS__> \
  bool operator< (Class const& lhs, Class const& rhs) { return lhs Extract <  rhs Extract ; } \
  template<__VA_ARGS__> \
  bool operator<=(Class const& lhs, Class const& rhs) { return lhs Extract <= rhs Extract ; } \
  template<__VA_ARGS__> \
  bool operator> (Class const& lhs, Class const& rhs) { return lhs Extract >  rhs Extract ; } \
  template<__VA_ARGS__> \
  bool operator>=(Class const& lhs, Class const& rhs) { return lhs Extract >= rhs Extract ; } \
  template<__VA_ARGS__> \
  bool operator==(Class const& lhs, Class const& rhs) { return lhs Extract == rhs Extract ; } \
  template<__VA_ARGS__> \
  bool operator!=(Class const& lhs, Class const& rhs) { return lhs Extract != rhs Extract ; }


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
  inline bool operator==(Collection const& lhs, Collection const& rhs) \
  { \
    using std::begin; \
    using std::end; \
    return std::equal( begin(lhs), end(lhs), begin(rhs), end(rhs) ); \
  } \
  inline bool operator!=(Collection const& lhs, Collection const& rhs) \
  { \
    return not ( lhs == rhs ); \
  } \
  inline bool operator<(Collection const& lhs, Collection const& rhs) \
  { \
    return std::lexicographical_compare( lhs.begin(), lhs.end(), rhs.begin(), rhs.end() ); \
  } \
  inline bool operator<=(Collection const& lhs, Collection const& rhs) \
  { \
    return lhs == rhs || lhs < rhs; \
  } \
  inline bool operator>(Collection const& lhs, Collection const& rhs) \
  { \
    return not ( lhs <= rhs ); \
  } \
  inline bool operator>=(Collection const& lhs, Collection const& rhs) \
  { \
    return not ( lhs < rhs ); \
  }


/** @brief helper macro to define all comparison operators, for a given collection template.
 *  <code>
 *  template<typename T, typename U = size_t>
 *  struct MyCollection
 *  {
 *    auto begin() { ... }
 *    auto end() { ... }
 *  }
 *  #define COLLECTION MyCollection<T,U>
 *  BUT_MPL_FREE_OPERATORS_COLLECTION_TEMPLATE_COMPARE(COLLECTION, typename T, typename U)
 *  #undef COLLECTION
 *  // ...
 *  const MyCollection<int> c1{1,2,3};
 *  const MyCollection<int> c2{2,3,4,5};
 *  assert( c1 < c2 );
 *  </code>
 *
 * @note for 1-argument template, it is enough to pass the type directly (i.e. w/o a helper macro):
 *  <code>
 *  template<typename T>
 *  struct MyCollection { ... };
 *  BUT_MPL_FREE_OPERATORS_COLLECTION_TEMPLATE_COMPARE(MyCollection<T>, typename T)
 *  </code>
 */
#define BUT_MPL_FREE_OPERATORS_COLLECTION_TEMPLATE_COMPARE(Collection, ...) \
  template<__VA_ARGS__> \
  bool operator==(Collection const& lhs, Collection const& rhs) \
  { \
    using std::begin; \
    using std::end; \
    return std::equal( begin(lhs), end(lhs), begin(rhs), end(rhs) ); \
  } \
  template<__VA_ARGS__> \
  bool operator!=(Collection const& lhs, Collection const& rhs) \
  { \
    return not ( lhs == rhs ); \
  } \
  template<__VA_ARGS__> \
  bool operator<(Collection const& lhs, Collection const& rhs) \
  { \
    return std::lexicographical_compare( lhs.begin(), lhs.end(), rhs.begin(), rhs.end() ); \
  } \
  template<__VA_ARGS__> \
  bool operator<=(Collection const& lhs, Collection const& rhs) \
  { \
    return lhs == rhs || lhs < rhs; \
  } \
  template<__VA_ARGS__> \
  bool operator>(Collection const& lhs, Collection const& rhs) \
  { \
    return not ( lhs <= rhs ); \
  } \
  template<__VA_ARGS__> \
  bool operator>=(Collection const& lhs, Collection const& rhs) \
  { \
    return not ( lhs < rhs ); \
  }
