#include <But/Log/Destination/JsonFile.hpp>
#include <But/Log/Field/Timestamp.hpp>
#include <But/Log/Field/LineNumber.hpp>
#include <But/Log/Field/Priority.hpp>
#include <But/Log/Field/FileName.hpp>

using namespace But::Log::Field;

int main()
{
  But::Log::Destination::JsonFile jf{"log.json"};
  jf.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "hello, world" );
  jf.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "bye, world" );

  jf.log( Priority::debug, "this is debug");
  jf.log( Priority::info, "this is info");
  jf.log( Priority::warning, "this is warning");
  jf.log( Priority::error, "this is error");

  jf.log( FormattedString{"foo"}, Priority::debug, "this is debug");
  jf.log( FormattedString{"foo"}, Priority::info, "this is info");
  jf.log( FormattedString{"foo"}, Priority::warning, "this is warning");
  jf.log( FormattedString{"foo"}, Priority::error, "this is error");

  But::Log::Destination::Sink& base = jf;
  base.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "hello, foreign world" );
  base.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "bye, foreign world" );

  base.log( Priority::debug, "this is debug");
  base.log( Priority::info, "this is info");
  base.log( Priority::warning, "this is warning");
  base.log( Priority::error, "this is error");

  jf.log("logging without", "priority");
  base.log("logging without", "priority,", "with base class");

  jf.flush();
  base.flush();

  jf.reload();
  base.reload();
}
