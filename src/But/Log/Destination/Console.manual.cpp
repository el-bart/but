#include "Console.hpp"
#include "But/Log/Field/Timestamp.hpp"
#include "But/Log/Field/LineNumber.hpp"
#include "But/Log/Field/Priority.hpp"
#include "But/Log/Field/FileName.hpp"

using namespace But::Log::Field;

int main()
{
  But::Log::Destination::Console c;
  c.log( Timestamp{}, " ", Priority::info, " @ ", FileName{__FILE__}, ":", LineNumber{__LINE__}, " ", "hello, world" );
  c.log( Timestamp{}, " ", Priority::info, " @ ", FileName{__FILE__}, ":", LineNumber{__LINE__}, " ", "bye, world" );
  c.flush();
  c.reload();

  But::Log::Destination::Foregin& base = c;
  base.log( Timestamp{}, " ", Priority::info, " @ ", FileName{__FILE__}, ":", LineNumber{__LINE__}, " ", "hello, foregin world" );
  base.log( Timestamp{}, " ", Priority::info, " @ ", FileName{__FILE__}, ":", LineNumber{__LINE__}, " ", "bye, foregin world" );
  base.flush();
  base.reload();
}
