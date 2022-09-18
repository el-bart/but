#include <But/Log/Destination/Syslog.hpp>
#include <But/Log/Field/Timestamp.hpp>
#include <But/Log/Field/LineNumber.hpp>
#include <But/Log/Field/Priority.hpp>
#include <But/Log/Field/FileName.hpp>
#include <But/Log/Destination/detail/args2FieldInfo.hpp>

using namespace But::Log::Field;
using But::Log::Destination::detail::args2FieldInfo;

int main()
{
  But::Log::Destination::Syslog s;
  s.log( args2FieldInfo( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "hello, world" ) );
  s.log( args2FieldInfo( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "bye, world" ) );

  s.log( args2FieldInfo( Priority::debug, "this is debug") );
  s.log( args2FieldInfo( Priority::info, "this is info") );
  s.log( args2FieldInfo( Priority::warning, "this is warning") );
  s.log( args2FieldInfo( Priority::error, "this is error") );

  s.log( args2FieldInfo( FormattedString{"debug format"}, Priority::debug, "this is debug" ) );
  s.log( args2FieldInfo( FormattedString{"info format"}, Priority::info, "this is info" ) );
  s.log( args2FieldInfo( FormattedString{"warning format"}, Priority::warning, "this is warning" ) );
  s.log( args2FieldInfo( FormattedString{"error format"}, Priority::error, "this is error" ) );

  But::Log::Destination::Sink& base = s;
  base.log( args2FieldInfo( Timestamp{}, Priority::info, "@", FileName{__FILE__}, ":", LineNumber{__LINE__}, "hello, foreign world" ) );
  base.log( args2FieldInfo( Timestamp{}, Priority::info, "@", FileName{__FILE__}, ":", LineNumber{__LINE__}, "bye, foreign world" ) );

  base.log( args2FieldInfo( Priority::debug, "this is debug" ) );
  base.log( args2FieldInfo( Priority::info, "this is info" ) );
  base.log( args2FieldInfo( Priority::warning, "this is warning" ) );
  base.log( args2FieldInfo( Priority::error, "this is error" ) );

  s.log( args2FieldInfo( "logging without", "priority" ) );
  base.log( args2FieldInfo( "logging without", "priority,", "with base class" ) );

  s.flush();
  base.flush();

  s.reload();
  base.reload();
}
