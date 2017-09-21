#include "JsonConsole.hpp"
#include "Common/toJsonStream.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

void JsonConsole::toStreamFormat(std::ostream& os, Backend::Entry const& entry)
{
  Common::toJsonStream(os, entry);
  os << endline();
}


void JsonConsole::toStreamFormat(std::ostream& os, Field::FormattedString const& str, Backend::Entry const& entry)
{
  Common::toJsonStream(os, str, entry);
  os << endline();
}

}
}
}
