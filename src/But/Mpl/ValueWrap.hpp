#pragma once
#include <But/Mpl/detail/ValueWrap.hpp>

namespace But::Mpl
{

/// @brief declares value that's both copyable and movable
#define BUT_MPL_VALUE_WRAP_CP_MV(name, type) \
  BUT_MPL_DETAIL_VALUE_WRAP(name, BUT_MPL_DETAIL_VALUE_WRAP_COPYABLE,     BUT_MPL_DETAIL_VALUE_WRAP_MOVABLE,     type)

/// @brief declares value that's copyable but not movable
#define BUT_MPL_VALUE_WRAP_CP(name, type) \
  BUT_MPL_DETAIL_VALUE_WRAP(name, BUT_MPL_DETAIL_VALUE_WRAP_COPYABLE,     BUT_MPL_DETAIL_VALUE_WRAP_NON_MOVABLE, type)

/// @brief declares value that's movable but not copyable
#define BUT_MPL_VALUE_WRAP_MV(name, type) \
  BUT_MPL_DETAIL_VALUE_WRAP(name, BUT_MPL_DETAIL_VALUE_WRAP_NON_COPYABLE, BUT_MPL_DETAIL_VALUE_WRAP_MOVABLE,     type)

}
