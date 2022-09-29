#include <But/assert.hpp>
#include <But/Log/Field/Priority.hpp>


namespace But
{
namespace Log
{
namespace Field
{

std::string_view fieldValue(const Priority pri)
{
  switch(pri)
  {
    case Priority::debug:   return "debug";
    case Priority::info:    return "info";
    case Priority::warning: return "warning";
    case Priority::error:   return "error";
  }
  assert(!"wrong priority value");
  abort();
}

}
}
}
