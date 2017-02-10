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


inline std::string toString(const LineNumber ln) { return std::to_string(ln.value_); }

}
}
}
