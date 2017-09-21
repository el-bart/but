#include <iostream>
#include <json.hpp>
#include "toJson.hpp"
#include "toJsonStream.hpp"

using json = nlohmann::json;

namespace But
{
namespace Log
{
namespace Destination
{
namespace Common
{

void toJsonStream(std::ostream& os, Backend::Entry const& entry)
{
  os << toJson(entry);
}


void toJsonStream(std::ostream& os, Field::FormattedString const& str, Backend::Entry const& entry)
{
  auto tab = json::array();
  tab.push_back( toJsonField( Backend::FieldInfo{str} ) );
  toJson(tab, entry);
  os << tab;
}

}
}
}
}
