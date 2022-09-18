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
  // note that direct stream writing, like this:
  //   os << toJson(entry);
  // is NOT possible, as for invalid UTF-8 characters it will throw, effectively creating invalid log entry (cut in the middle).
  // it will be possible to get back to direct stream writing, once provided with invalid UTF-8 characters replacer in string data.
  constexpr auto indent = -1;
  constexpr auto indentChar = ' ';
  constexpr auto ensureAscii = false;
  constexpr auto errorHandler = nlohmann::json::error_handler_t::replace;
  os << toJson(entry).dump(indent, indentChar, ensureAscii, errorHandler);
}

}
}
}
}
