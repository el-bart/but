#include "JsonFile.hpp"
#include "Common/toJsonStream.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

void JsonFile::toStreamFormat(std::ostream& os, Backend::Entry const& entry)
{
  Common::toJsonStream(os, entry);
  os << endline();
}


void JsonFile::toStreamFormat(std::ostream& os, Field::FormattedString const& str, Backend::Entry const& entry)
{
  Common::toJsonStream(os, str, entry);
  os << endline();
}

}
}
}
