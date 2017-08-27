#pragma once
#include "But/Format/Parsed.hpp"

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
  template<unsigned N, unsigned M>
  auto translate(Format::Parsed<N,M>&& parsed) const { return std::move(parsed); }
};

}
}
}
