#include "But/assert.hpp"
#include <syslog.h>
#include "But/Log/Backend/toValue.hpp"
#include "Syslog.hpp"

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
  if( str == toValue(Field::Priority::debug) )   return Field::Priority::debug;
  if( str == toValue(Field::Priority::info) )    return Field::Priority::info;
  if( str == toValue(Field::Priority::warning) ) return Field::Priority::warning;
  if( str == toValue(Field::Priority::error) )   return Field::Priority::error;
  BUT_ASSERT(!"unknown priority string");
  throw std::logic_error{"unknown priority string: " + str};
}
}

void Syslog::logImpl(Backend::Entry const& e)
{
  auto pri = toValue(Field::Priority::info);
  std::stringstream ss;
  for(auto& f: e)
  {
    ss << trim_( f.value() );
    if( f.type() == toType(Field::Priority::info) )
      pri = f.value();
  }
  const auto p = stringToPriority(pri);
  const auto slp = toSyslogPriority(p);
  syslog( slp, "%s", ss.str().c_str() );
}


void Syslog::logImpl(Field::FormattedString const& str, Backend::Entry const& e)
{
  auto pri = toValue(Field::Priority::info);
  for(auto& f: e)
  {
    if( f.type() == toType(Field::Priority::info) )
      pri = f.value();
  }
  const auto p = stringToPriority(pri);
  const auto slp = toSyslogPriority(p);
  const auto trimmed = trim_(str.value_);
  syslog( slp, "%s", trimmed.c_str() );
}

}
}
}
