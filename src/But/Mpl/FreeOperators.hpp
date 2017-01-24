#pragma once

#define BUT_MPL_FREE_OPERATORS_COMPARE(Class, Extract) \
  inline bool operator< (Class const& lhs, Class const& rhs) { return lhs Extract <  rhs Extract ; } \
  inline bool operator<=(Class const& lhs, Class const& rhs) { return lhs Extract <= rhs Extract ; } \
  inline bool operator> (Class const& lhs, Class const& rhs) { return lhs Extract >  rhs Extract ; } \
  inline bool operator>=(Class const& lhs, Class const& rhs) { return lhs Extract >= rhs Extract ; } \
  inline bool operator==(Class const& lhs, Class const& rhs) { return lhs Extract == rhs Extract ; } \
  inline bool operator!=(Class const& lhs, Class const& rhs) { return lhs Extract != rhs Extract ; }
