#pragma once
#include <But/Format/Parsed.hpp>

namespace But
{
namespace Log
{
namespace Localization
{

/** @brief stub object, providing zero-overhead non-translating implementation. it is meant to be used, when logs
 *         localization is not needed.
 */
struct None final
{
  auto operator->() const { return this; }
  template<size_t N, size_t M>
  auto translate(Format::ParsedCompiletime<N,M> const& parsed) const { return parsed; }
};

}
}
}
