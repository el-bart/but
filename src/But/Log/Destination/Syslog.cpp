#include <But/Log/Destination/Syslog.hpp>
#include <But/Log/Field/LogLevel.hpp>
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
auto toSyslogLogLevel(const Field::LogLevel p)
{
  switch(p)
  {
    case Field::LogLevel::debug:   return LOG_DEBUG;
    case Field::LogLevel::info:    return LOG_INFO;
    case Field::LogLevel::warning: return LOG_WARNING;
    case Field::LogLevel::error:   return LOG_ERR;
  }
  BUT_ASSERT(!"unknown priority");
  throw std::logic_error{"unknown priority"};
}

auto stringToLogLevel(std::string const& str)
{
  static auto const prioFieldTag = std::string{ fieldName(static_cast<Field::LogLevel*>(nullptr)) };
  // TODO: this should be doable with sax parser, instead
  auto const j = nlohmann::json::parse(str);
  auto const it = j.find(prioFieldTag);
  if( it == end(j) ) return Field::LogLevel::info;   // default version

  std::string const& value = *it;
  for(auto p: {
        Field::LogLevel::debug,
        Field::LogLevel::info,
        Field::LogLevel::warning,
        Field::LogLevel::error })
    if( value == fieldValue(p) )
      return p;
  BUT_ASSERT(!"unknown priority string");
  throw std::logic_error{"unknown priority string: " + value};
}
}

void Syslog::logImpl(std::string&& str)
{
  auto const prio = stringToLogLevel(str);
  auto const slPrio = toSyslogLogLevel(prio);
  auto const trimmedStr = trim_( std::move(str) );
  syslog( slPrio, "%s", trimmedStr.c_str() );
}

}
}
}
