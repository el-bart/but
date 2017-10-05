#include <iostream>
#include <json.hpp>
#include "toJson.hpp"
#include "toJsonStream.hpp"

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
