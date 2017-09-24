#pragma once
#include <thread>
#include "But/Log/Backend/Type.hpp"
#include "But/Log/Backend/Value.hpp"

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

inline auto toType(ThreadNo const&) { return Backend::Type{"But::ThreadNo"}; }
inline auto toValue(ThreadNo const& tno) { return Backend::Value{tno.value_}; }

}
}
}
