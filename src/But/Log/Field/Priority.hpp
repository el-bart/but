#pragma once
#include <But/Log/Backend/FieldInfo.hpp>

namespace But
{
namespace Log
{
namespace Field
{

enum class Priority
{
  debug   = 0,
  info    = 1,
  warning = 2,
  error   = 3
};

Backend::FieldInfo toFieldInfo(Priority pri);

}
}
}
