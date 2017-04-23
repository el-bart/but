#pragma once
#include "But/Container/Array.hpp"

namespace But
{
namespace Format
{
namespace detail
{

struct State
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
struct ParserState final
{
  Container::Array<State,N> segments_;
  unsigned count_{0};
};

}
}
}
