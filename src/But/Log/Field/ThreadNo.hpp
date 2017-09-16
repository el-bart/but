#pragma once
#include <string>
#include <thread>

namespace But
{
namespace Log
{
namespace Field
{

struct ThreadNo final
{
  ThreadNo();
  uint64_t value_;
};

inline std::string toType(ThreadNo const&) { return "But::ThreadNo"; }
inline std::string toValue(ThreadNo const& tno) { return std::to_string(tno.value_); }

}
}
}
