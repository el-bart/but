#pragma once
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

template<unsigned N>
struct ParsedFormat final
{
  Container::ArrayWithSize<Segment,N> segments_;
};

}
}
}
