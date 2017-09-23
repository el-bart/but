#pragma once
#include <string>
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


inline std::string toType(LineNumber const&) { return "But::LineNumber"; }
inline Backend::Value toValue(const LineNumber ln) { return Backend::Value{ln.value_}; }

}
}
}
