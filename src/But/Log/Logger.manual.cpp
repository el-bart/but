#include <But/Log/Logger.hpp>
#include <But/Log/Destination/Console.hpp>
#include <But/Log/Field/Timestamp.hpp>
#include <But/Log/Field/LineNumber.hpp>
#include <But/Log/Field/LogLevel.hpp>
#include <But/Log/Field/FileName.hpp>

using namespace But::Log::Field;

int main()
{
  But::Log::Logger<> p{ But::makeSharedNN<But::Log::Destination::Console>() };
  p.log( "hello, world", Timestamp{}, LogLevel::info, FileName{__FILE__}, LineNumber{__LINE__} );
  p.log( "byte, world", Timestamp{}, LogLevel::info, FileName{__FILE__}, LineNumber{__LINE__} );
  p.log( BUT_FORMAT("message generated from ${0}:${1}"), FileName{__FILE__}, LineNumber{__LINE__} );
  p.flush();
  p.reload();
}
