#include "Syslog.hpp"
#include "But/Log/Field/Timestamp.hpp"
#include "But/Log/Field/LineNumber.hpp"
#include "But/Log/Field/Priority.hpp"
#include "But/Log/Field/FileName.hpp"

using namespace But::Log::Field;

int main()
{
  But::Log::Destination::Syslog s;
  s.log( Timestamp{}, " ", Priority::info, " @ ", FileName{__FILE__}, ":", LineNumber{__LINE__}, " ", "hello, world" );
  s.log( Timestamp{}, " ", Priority::info, " @ ", FileName{__FILE__}, ":", LineNumber{__LINE__}, " ", "bye, world" );

  But::Log::Destination::Foregin& base = s;
  base.log( Timestamp{}, " ", Priority::info, " @ ", FileName{__FILE__}, ":", LineNumber{__LINE__}, " ", "hello, foregin world" );
  base.log( Timestamp{}, " ", Priority::info, " @ ", FileName{__FILE__}, ":", LineNumber{__LINE__}, " ", "bye, foregin world" );

  s.log("logging without ", " priority");
  base.log("logging without ", " priority", ", with base class");
}
