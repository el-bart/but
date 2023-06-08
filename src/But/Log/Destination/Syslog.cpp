#include <But/Log/Destination/Syslog.hpp>
#include <But/Log/Field/Priority.hpp>
#include <But/assert.hpp>
#include <syslog.h>
#include <nlohmann/json.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

namespace
{
auto toSyslogPriority(const Field::Priority p)
{
  switch(p)
  {
    case Field::Priority::debug:   return LOG_DEBUG;
    case Field::Priority::info:    return LOG_INFO;
    case Field::Priority::warning: return LOG_WARNING;
    case Field::Priority::error:   return LOG_ERR;
  }
  BUT_ASSERT(!"unknown priority");
  throw std::logic_error{"unknown priority"};
}

auto stringToPriority(std::string const& str)
{
  static auto const prioFieldTag = std::string{ fieldName(static_cast<Field::Priority*>(nullptr)) };
  // TODO: this should be doable with sax parser, instead
  auto const j = nlohmann::json::parse(str);
  auto const it = j.find(prioFieldTag);
  if( it == end(j) ) return Field::Priority::info;   // default version

  std::string const& value = *it;
  for(auto p: {
        Field::Priority::debug,
        Field::Priority::info,
        Field::Priority::warning,
        Field::Priority::error })
    if( value == fieldValue(p) )
      return p;
  BUT_ASSERT(!"unknown priority string");
  throw std::logic_error{"unknown priority string: " + value};
}
}

void Syslog::logImpl(std::string&& str)
{
  auto const prio = stringToPriority(str);
  auto const slPrio = toSyslogPriority(prio);
  auto const trimmedStr = trim_( std::move(str) );
  syslog( slPrio, "%s", trimmedStr.c_str() );
}

}
}
}
