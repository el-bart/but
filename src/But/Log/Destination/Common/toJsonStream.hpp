#pragma once
#include <iosfwd>
#include "But/Log/Backend/FieldInfo.hpp"

namespace But
{
namespace Log
{
namespace Destination
{
namespace Common
{

void toJsonStream(std::ostream& os, Backend::FieldInfo const& entry);

}
}
}
}
