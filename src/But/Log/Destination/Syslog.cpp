#include <cassert>
#include <syslog.h>
#include "But/Log/Backend/toString.hpp"
#include "But/Log/Backend/trimNonPrintable.hpp"
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
  assert(!"unknown priority");
  throw std::logic_error{"unknown priority"};
}
}

void Syslog::append(std::stringstream& ss, const Field::Priority p)
{
  const auto slp = toSyslogPriority(p);
  syslog( slp, "%s", ss.str().c_str() );
}


namespace
{
auto stringToPriority(std::string const& str)
{
  if( str == toString(Field::Priority::debug) )   return Field::Priority::debug;
  if( str == toString(Field::Priority::info) )    return Field::Priority::info;
  if( str == toString(Field::Priority::warning) ) return Field::Priority::warning;
  if( str == toString(Field::Priority::error) )   return Field::Priority::error;
  assert(!"unknown priority string");
  throw std::logic_error{"unknown priority string: " + str};
}
}

void Syslog::logImpl(Backend::Entry e)
{
  auto pri = toString(Field::Priority::info);
  std::stringstream ss;
  for(auto& f: e)
  {
    ss << Backend::trimNonPrintable( f.value() );
    if( f.type() == typeString(Field::Priority::info) )
      pri = f.value();
  }
  const auto p = stringToPriority(pri);
  const auto slp = toSyslogPriority(p);
  syslog( slp, "%s", ss.str().c_str() );
}

}
}
}
