#pragma once
#include <string>
#include <sys/types.h>
#include <unistd.h>
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

inline std::string toType(Pid const&) { return "But::Pid"; }
inline Backend::Value toValue(Pid const& pid) { return Backend::Value{pid.value_}; }

}
}
}
