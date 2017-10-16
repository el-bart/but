#include <But/Log/Destination/JsonFile.hpp>
#include <But/Log/Destination/Common/toJsonStream.hpp>

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
