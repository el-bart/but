#include <But/assert.hpp>
#include <But/Log/Field/LogLevel.hpp>


namespace But
{
namespace Log
{
namespace Field
{

std::string_view fieldValue(const LogLevel pri)
{
  switch(pri)
  {
    case LogLevel::debug:   return "debug";
    case LogLevel::info:    return "info";
    case LogLevel::warning: return "warning";
    case LogLevel::error:   return "error";
  }
  assert(!"wrong log level value");
  abort();
}

}
}
}
