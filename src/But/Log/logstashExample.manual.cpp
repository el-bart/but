#include <chrono>
#include <thread>
#include <random>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <But/Log/Destination/Tcp.hpp>
#include <But/Log/Destination/MultiSink.hpp>
#include <But/Log/Destination/Console.hpp>
#include <But/Log/Destination/BackgroundThread.hpp>
#include <But/Log/Logger.hpp>
#include <But/Log/Field/Timestamp.hpp>
#include <But/Log/Field/LineNumber.hpp>
#include <But/Log/Field/Priority.hpp>
#include <But/Log/Field/FileName.hpp>

using namespace But::Log::Field;
using But::Log::Backend::EntryProxy;
using Clock = std::chrono::steady_clock;


struct Component
{
  std::string name_;
};
constexpr auto fieldName(Component const*) { return "Component"; }
auto fieldValue(Component const& c) { return std::string_view{c.name_}; }


struct Point
{
  int x_;
  int y_;
};
constexpr auto fieldName(Point const*) { return "Point"; }
void objectValue(EntryProxy& p, Point const& pt)
{
  p.value("x", pt.x_);
  p.value("y", pt.y_);
}


struct Line
{
  Point from_;
  Point to_;
};
constexpr auto fieldName(Line const*) { return "Line"; }
void objectValue(EntryProxy& p, Line const& l)
{
  p.object("from").nest(l.from_);
  p.object("to").nest(l.to_);
}


auto makeLogger(std::string host, const uint16_t port)
{
  auto tcp = But::makeSharedNN<But::Log::Destination::Tcp>( std::move(host), port );
  auto console = But::makeSharedNN<But::Log::Destination::Console>();
  auto multi = But::makeSharedNN<But::Log::Destination::MultiSink>( std::move(console), std::move(tcp) );
  auto bgThread = But::makeSharedNN<But::Log::Destination::BackgroundThread>( std::move(multi), 2*1000 );
  But::Log::Logger<> log{ std::move(bgThread) };
  return log;
}


struct Prng
{
  auto operator()() { return dist_(prng_); }

private:
  std::mt19937 prng_{ std::random_device{}() };
  std::uniform_int_distribution<> dist_{0, 3};
};


int main(int argc, char** argv)
{
  if(argc!=1+2)
  {
    std::cerr << argv[0] << " <host> <port>" << std::endl;
    return 2;
  }

  auto log = makeLogger( argv[1], boost::lexical_cast<uint16_t>(argv[2]) );
  auto prng = Prng{};
  constexpr auto logDelay = std::chrono::milliseconds{200};
  constexpr auto flushDelay = std::chrono::seconds{1};

  auto flushDeadline = Clock::now() + flushDelay;
  while(true)
  {
    const auto deadline = Clock::now() + logDelay;
    switch( prng() )
    {
      case 0:
        log.log( "hello, world", Timestamp{}, Priority::info, FileName{__FILE__}, LineNumber{__LINE__}, Component{"FooBar"} );
        break;
      case 1:
        log.log( "other case here", Timestamp{}, Priority::debug, Component{"point-hair boss"}, Point{13, 42} );
        break;
      case 2:
        log.log( "yet again sth new", Timestamp{}, Priority::debug, Component{"another-point"}, Point{12, 41} );
        break;
      case 3:
        log.log( "lining up", Priority::warning, Component{"line man"}, Line{ Point{1, 2}, Point{3, 4} } );
        break;
    }

    if( Clock::now() >= flushDeadline )
    {
      log.flush();
      flushDeadline += flushDelay;
    }

    std::this_thread::sleep_until(deadline);

    // NOTE: you can also play with:
    // log.reload();
    // to simulate dosconnections every now and then...
  }
}
