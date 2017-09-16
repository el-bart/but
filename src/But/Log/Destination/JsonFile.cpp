#include "JsonFile.hpp"
#include "Common/toJson.hpp"

using json = nlohmann::json;

namespace But
{
namespace Log
{
namespace Destination
{

void JsonFile::toStreamFormat(std::ostream& os, Backend::Entry const& entry)
{
  auto tab = Common::toJson(entry);
  os << tab << "\n";
}


void JsonFile::toStreamFormat(std::ostream& os, Field::FormattedString const& str, Backend::Entry const& entry)
{
  auto tab = json::array();
  tab.push_back( Common::toJsonField( Backend::FieldInfo{str} ) );
  Common::toJson(tab, entry);
  os << tab << "\n";
}

}
}
}
