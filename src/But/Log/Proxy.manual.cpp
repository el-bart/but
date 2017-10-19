#include <But/Log/Proxy.hpp>
#include <But/Log/Destination/TextConsole.hpp>
#include <But/Log/Field/Timestamp.hpp>
#include <But/Log/Field/LineNumber.hpp>
#include <But/Log/Field/Priority.hpp>
#include <But/Log/Field/FileName.hpp>

using namespace But::Log::Field;

int main()
{
  But::Log::Proxy<> p{ But::makeSharedNN<But::Log::Destination::TextConsole>() };
  p.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "hello, world" );
  p.log( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, "bye, world" );
  p.log( BUT_FORMAT("format test of $0"), "whatever..." );
  p.log( Timestamp{}, Priority::warning, ':', BUT_FORMAT("trailing args $0"), "included" );
  p.flush();
  p.reload();
}
