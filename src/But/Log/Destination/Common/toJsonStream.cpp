#include <iostream>
#include <nlohmann/json.hpp>
#include <But/Log/Destination/Common/toJson.hpp>
#include <But/Log/Destination/Common/toJsonStream.hpp>

namespace But
{
namespace Log
{
namespace Destination
{
namespace Common
{

void toJsonStream(std::ostream& os, Backend::FieldInfo const& entry)
{
  os << toJson(entry);
}

}
}
}
}
