#pragma once
#include <sys/types.h>
#include <unistd.h>
#include "But/Log/Backend/Type.hpp"
#include "But/Log/Backend/Value.hpp"

namespace But
{
namespace Log
{
namespace Field
{

struct Pid final
{
  pid_t value_{ getpid() };
};

inline auto toType(Pid const&) { return Backend::Type{"But::Pid"}; }
inline auto toValue(Pid const& pid) { return Backend::Value{pid.value_}; }

}
}
}
