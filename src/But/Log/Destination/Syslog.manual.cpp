#include <But/Log/Destination/Syslog.hpp>
#include <But/Log/Field/Timestamp.hpp>
#include <But/Log/Field/LineNumber.hpp>
#include <But/Log/Field/Priority.hpp>
#include <But/Log/Field/FileName.hpp>
#include <nlohmann/json.hpp>

using namespace But::Log::Field;

namespace
{
auto logWithPrio(Priority p)
{
  nlohmann::json j;
  j[ std::string{ fieldName(&p) } ] = fieldValue(p);
  j["foo"] = "bar";
  return j.dump();
}
}

int main()
{
  But::Log::Destination::Syslog s;

  s.log( logWithPrio(Priority::debug)   );
  s.log( logWithPrio(Priority::info)    );
  s.log( logWithPrio(Priority::warning) );
  s.log( logWithPrio(Priority::error)   );

  s.log( "{\"log\":\"w/o prio\"}" );

  s.flush();
  s.reload();

  But::Log::Destination::Sink& base = s;

  base.log( logWithPrio(Priority::debug)   );
  base.log( logWithPrio(Priority::info)    );
  base.log( logWithPrio(Priority::warning) );
  base.log( logWithPrio(Priority::error)   );

  base.log( "{\"log\":\"w/o prio\"}" );

  base.flush();
  base.reload();
}
