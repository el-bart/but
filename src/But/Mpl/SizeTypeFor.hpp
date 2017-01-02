#pragma once
#include <limits>
#include <type_traits>
#include <cinttypes>

namespace But
{
namespace Mpl
{

/** @brief calculates smalles possible size to keep values up to N.
 */
template<uint64_t N>
struct SizeTypeFor final
{
  using type =
    typename std::conditional_t< ( N <= std::numeric_limits< uint8_t>::max() ),   uint8_t,
    typename std::conditional_t< ( N <= std::numeric_limits<uint16_t>::max() ),  uint16_t,
    typename std::conditional_t< ( N <= std::numeric_limits<uint32_t>::max() ),  uint32_t,
                                                                                 uint64_t
    >>>;
};

}
}
