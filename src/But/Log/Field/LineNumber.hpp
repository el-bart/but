#pragma once
#include <string>

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


inline auto fieldName(LineNumber const*) { return std::string_view{"But::LineNumber"}; }
inline auto fieldValue(LineNumber const& o) { return o.value_; }

}
}
}
