#pragma once
#include "But/Log/Backend/Type.hpp"
#include "But/Log/Backend/Value.hpp"

namespace But
{
namespace Log
{
namespace Field
{

struct LineNumber final
{
  unsigned value_;
};


inline auto toType(LineNumber const&) { return Backend::Type{"But::LineNumber"}; }
inline auto toValue(const LineNumber ln) { return Backend::Value{ln.value_}; }

}
}
}
