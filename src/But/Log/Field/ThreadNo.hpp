#pragma once
#include <thread>
#include <string>

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

inline auto fieldName(ThreadNo const*) { return std::string_view{"But::ThreadNo"}; }
inline auto fieldValue(const ThreadNo o) { return o.value_; }

}
}
}
