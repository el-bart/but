#pragma once
#include <sys/types.h>
#include <unistd.h>
#include "But/Log/Backend/FieldInfo.hpp"

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

inline auto toFieldInfo(const Pid pid) { return Backend::FieldInfo{ Backend::Type{"But::Pid"}, Backend::Value{pid.value_} }; }

}
}
}
