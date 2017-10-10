#pragma once
#include "But/Log/Backend/FieldInfo.hpp"

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


inline auto toFieldInfo(const LineNumber ln) { return Backend::FieldInfo{ Backend::Tag{"But::LineNumber"}, Backend::Value{ln.value_} }; }

}
}
}
