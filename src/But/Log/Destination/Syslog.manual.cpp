#include "Syslog.hpp"
#include "But/Log/Field/Timestamp.hpp"
#include "But/Log/Field/LineNumber.hpp"
#include "But/Log/Field/Priority.hpp"
#include "But/Log/Field/FileName.hpp"

using namespace But::Log::Field;

int main()
{
  But::Log::Destination::Syslog s;
  s.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "hello, world" );
  s.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "bye, world" );

  s.log( Priority::debug, "this is debug");
  s.log( Priority::info, "this is info");
  s.log( Priority::warning, "this is warning");
  s.log( Priority::error, "this is error");

  s.log( FormattedString{"debug format"}, Priority::debug, "this is debug" );
  s.log( FormattedString{"info format"}, Priority::info, "this is info" );
  s.log( FormattedString{"warning format"}, Priority::warning, "this is warning" );
  s.log( FormattedString{"error format"}, Priority::error, "this is error" );

  But::Log::Destination::Sink& base = s;
  base.log( Timestamp{}, Priority::info, "@", FileName{__FILE__}, ":", LineNumber{__LINE__}, "hello, foreign world" );
  base.log( Timestamp{}, Priority::info, "@", FileName{__FILE__}, ":", LineNumber{__LINE__}, "bye, foreign world" );

  base.log( Priority::debug, "this is debug");
  base.log( Priority::info, "this is info");
  base.log( Priority::warning, "this is warning");
  base.log( Priority::error, "this is error");

  s.log("logging without", "priority");
  base.log("logging without", "priority,", "with base class");

  s.flush();
  base.flush();

  s.reload();
  base.reload();
}
