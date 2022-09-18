#include <But/Log/Destination/JsonFile.hpp>
#include <But/Log/Field/Timestamp.hpp>
#include <But/Log/Field/LineNumber.hpp>
#include <But/Log/Field/Priority.hpp>
#include <But/Log/Field/FileName.hpp>
#include <But/Log/Destination/detail/args2FieldInfo.hpp>

using But::Log::Destination::detail::args2FieldInfo;

using namespace But::Log::Field;

int main()
{
  But::Log::Destination::JsonFile jf{"log.json"};
  jf.log( args2FieldInfo( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "hello, world" ) );
  jf.log( args2FieldInfo( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "bye, world" ) );

  jf.log( args2FieldInfo( Priority::debug, "this is debug") );
  jf.log( args2FieldInfo( Priority::info, "this is info") );
  jf.log( args2FieldInfo( Priority::warning, "this is warning") );
  jf.log( args2FieldInfo( Priority::error, "this is error") );

  jf.log( args2FieldInfo( FormattedString{"foo"}, Priority::debug, "this is debug") );
  jf.log( args2FieldInfo( FormattedString{"foo"}, Priority::info, "this is info") );
  jf.log( args2FieldInfo( FormattedString{"foo"}, Priority::warning, "this is warning") );
  jf.log( args2FieldInfo( FormattedString{"foo"}, Priority::error, "this is error") );

  But::Log::Destination::Sink& base = jf;
  base.log( args2FieldInfo( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "hello, foreign world" ) );
  base.log( args2FieldInfo( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "bye, foreign world" ) );

  base.log( args2FieldInfo( Priority::debug, "this is debug") );
  base.log( args2FieldInfo( Priority::info, "this is info") );
  base.log( args2FieldInfo( Priority::warning, "this is warning") );
  base.log( args2FieldInfo( Priority::error, "this is error") );

  jf.log( args2FieldInfo( "logging without", "priority" ) );
  base.log( args2FieldInfo( "logging without", "priority,", "with base class") );

  jf.flush();
  base.flush();

  jf.reload();
  base.reload();
}
