#pragma once
#include <string>
#include <thread>
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

inline std::string toType(ThreadNo const&) { return "But::ThreadNo"; }
inline Backend::Value toValue(ThreadNo const& tno) { return Backend::Value{tno.value_}; }

}
}
}
