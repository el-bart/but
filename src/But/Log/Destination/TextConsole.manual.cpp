#include "TextConsole.hpp"
#include "But/Log/Field/Timestamp.hpp"
#include "But/Log/Field/LineNumber.hpp"
#include "But/Log/Field/Priority.hpp"
#include "But/Log/Field/FileName.hpp"

using namespace But::Log::Field;

int main()
{
  But::Log::Destination::TextConsole c;
  c.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "hello, world" );
  c.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "bye, world" );
  c.log( FormattedString{"format test"}, "whatever..." );
  c.flush();
  c.reload();

  But::Log::Destination::Foreign& base = c;
  base.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "hello, foreign world" );
  base.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "bye, foreign world" );
  base.flush();
  base.reload();
}
