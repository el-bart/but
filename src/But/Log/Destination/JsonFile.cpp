#include "JsonFile.hpp"
#include "Common/toJsonStream.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

void JsonFile::toStreamFormat(std::ostream& os, Backend::FieldInfo const& fi)
{
  Common::toJsonStream(os, fi);
  os << endline();
}

}
}
}
