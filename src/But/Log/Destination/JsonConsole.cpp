#include <iostream>
#include "JsonConsole.hpp"
#include "Common/toJsonStream.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

JsonConsole::JsonConsole(const bool syncWithStdio):
  TextStream{std::cout}
{
  std::cout.sync_with_stdio(syncWithStdio);
}

void JsonConsole::toStreamFormat(std::ostream& os, Backend::FieldInfo const& fi)
{
  Common::toJsonStream(os, fi);
  os << endline();
}

}
}
}
