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

void Syslog::append(std::stringstream& ss)
{
  // TODO: priority...
  syslog( LOG_INFO, "%s", ss.str().c_str() );
}

void Syslog::logImpl(Backend::Entry e)
{
  std::stringstream ss;
  for(auto& f: e)
    ss << Backend::trimNonPrintable( f.value() );
  //send(ss
}

}
}
}
