#pragma once
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

inline std::string typeString(ThreadNo const&) { return "But::ThreadNo"; }
inline std::string toString(ThreadNo const& tno) { return std::to_string(tno.value_); }

}
}
}
