#pragma once

/** @brief heklper macro to define all comparison operators, for a given class, based on a provided extract operator.
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

// TODO: template
#define BUT_MPL_FREE_OPERATORS_TEMPLATE_COMPARE(Class, ...) \
  inline bool operator< (Class const& lhs, Class const& rhs) { return lhs Extract <  rhs Extract ; } \
  inline bool operator<=(Class const& lhs, Class const& rhs) { return lhs Extract <= rhs Extract ; } \
  inline bool operator> (Class const& lhs, Class const& rhs) { return lhs Extract >  rhs Extract ; } \
  inline bool operator>=(Class const& lhs, Class const& rhs) { return lhs Extract >= rhs Extract ; } \
  inline bool operator==(Class const& lhs, Class const& rhs) { return lhs Extract == rhs Extract ; } \
  inline bool operator!=(Class const& lhs, Class const& rhs) { return lhs Extract != rhs Extract ; }
