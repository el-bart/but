#include <But/Log/Destination/Syslog.hpp>
#include <But/Log/Field/Timestamp.hpp>
#include <But/Log/Field/LineNumber.hpp>
#include <But/Log/Field/LogLevel.hpp>
#include <But/Log/Field/FileName.hpp>
#include <nlohmann/json.hpp>

using namespace But::Log::Field;

namespace
{
auto logWithPrio(LogLevel p)
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

  s.log( logWithPrio(LogLevel::debug)   );
  s.log( logWithPrio(LogLevel::info)    );
  s.log( logWithPrio(LogLevel::warning) );
  s.log( logWithPrio(LogLevel::error)   );

  s.log( "{\"log\":\"w/o prio\"}" );

  s.flush();
  s.reload();

  But::Log::Destination::Sink& base = s;

  base.log( logWithPrio(LogLevel::debug)   );
  base.log( logWithPrio(LogLevel::info)    );
  base.log( logWithPrio(LogLevel::warning) );
  base.log( logWithPrio(LogLevel::error)   );

  base.log( "{\"log\":\"w/o prio\"}" );

  base.flush();
  base.reload();
}
