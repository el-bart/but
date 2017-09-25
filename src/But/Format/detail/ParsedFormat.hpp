#pragma once
#include <vector>
#include "But/Container/ArrayWithSize.hpp"

namespace But
{
namespace Format
{
namespace detail
{

struct Segment final
{
  enum class Type
  {
    String,
    Value,
    TypeName
  };

  char const* begin_{nullptr};
  char const* end_{nullptr};
  Type type_{Type::String};
  unsigned referencedArgument_{0};  // only valid for Value and TypeName types
};

template<typename C>
struct ParsedFormat final
{
  constexpr auto size() const { return segments_.size(); }
  C segments_;
};

template<size_t N>
using ParsedFormatCt = ParsedFormat< Container::ArrayWithSize<Segment,N> >;

using ParsedFormatRt = ParsedFormat< std::vector<Segment> >;

}
}
}
