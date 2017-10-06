#include "But/assert.hpp"
#include <syslog.h>
#include "But/Log/Field/Priority.hpp"
#include "Syslog.hpp"
#include "detail/StreamAndTrimVisitor.hpp"

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
  if( str == toFieldInfo(Field::Priority::debug).value().get<std::string>() )   return Field::Priority::debug;
  if( str == toFieldInfo(Field::Priority::info).value().get<std::string>() )    return Field::Priority::info;
  if( str == toFieldInfo(Field::Priority::warning).value().get<std::string>() ) return Field::Priority::warning;
  if( str == toFieldInfo(Field::Priority::error).value().get<std::string>() )   return Field::Priority::error;
  BUT_ASSERT(!"unknown priority string");
  throw std::logic_error{"unknown priority string: " + str};
}

auto extractSyslogPriority(Backend::FieldInfo const& fi)
{
  const auto prioType = toFieldInfo(Field::Priority::info).type();
  for( auto& f: fi.array() )
  {
    if( f.type() != prioType )
      continue;
    const auto p = stringToPriority( f.value().get<std::string>() );
    return toSyslogPriority(p);
  }
  return toSyslogPriority(Field::Priority::info);
}
}

void Syslog::logImpl(Backend::FieldInfo const& fi)
{
  std::stringstream ss;
  detail::StreamAndTrimVisitor satv{&trim_, &ss};
  fi.visit(satv);
  const auto slp = extractSyslogPriority(fi);
  syslog( slp, "%s", ss.str().c_str() );
}

}
}
}
