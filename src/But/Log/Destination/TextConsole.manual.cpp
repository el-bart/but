#include <But/Log/Destination/TextConsole.hpp>
#include <But/Log/Field/Timestamp.hpp>
#include <But/Log/Field/LineNumber.hpp>
#include <But/Log/Field/Priority.hpp>
#include <But/Log/Field/FileName.hpp>
#include <But/Log/Destination/detail/args2FieldInfo.hpp>

using But::Log::Destination::detail::args2FieldInfo;
using namespace But::Log::Field;

int main()
{
  But::Log::Destination::TextConsole c;
  c.log( args2FieldInfo( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "hello, world" ) );
  c.log( args2FieldInfo( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "bye, world" ) );
  c.log( args2FieldInfo( FormattedString{"format test"}, "whatever..." ) );
  c.flush();
  c.reload();

  But::Log::Destination::Sink& base = c;
  base.log( args2FieldInfo( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "hello, foreign world" ) );
  base.log( args2FieldInfo( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "bye, foreign world" ) );
  base.flush();
  base.reload();
}
