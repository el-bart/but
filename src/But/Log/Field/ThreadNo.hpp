#pragma once
#include <thread>
#include "But/Log/Backend/FieldInfo.hpp"

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

inline auto toFieldInfo(const ThreadNo tno) { return Backend::FieldInfo{ Backend::Type{"But::ThreadNo"}, Backend::Value{tno.value_} }; }

}
}
}
