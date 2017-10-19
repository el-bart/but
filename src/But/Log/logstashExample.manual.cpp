#include <chrono>
#include <thread>
#include <random>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <But/Log/Destination/JsonTcp.hpp>
#include <But/Log/Destination/MultiSink.hpp>
#include <But/Log/Destination/TextConsole.hpp>
#include <But/Log/Destination/BackgroundThread.hpp>
#include <But/Log/Proxy.hpp>
#include <But/Log/Field/Timestamp.hpp>
#include <But/Log/Field/LineNumber.hpp>
#include <But/Log/Field/Priority.hpp>
#include <But/Log/Field/FileName.hpp>
#include <But/Log/Destination/detail/args2FieldInfo.hpp>

using But::Log::Destination::detail::args2FieldInfo;

using namespace But::Log::Field;
using But::Log::Backend::Tag;
using But::Log::Backend::FieldInfo;
using Clock = std::chrono::steady_clock;


struct Component
{
  std::string name_;
};

auto toFieldInfo(Component const& c)
{
  using But::Log::Backend::toFieldInfo;
  return toFieldInfo(c.name_).retag(Tag{"Component"});
}

struct Point
{
  int x_;
  int y_;
};

auto toFieldInfo(Point const& p)
{
  using But::Log::Backend::toFieldInfo;
  return FieldInfo{ Tag{"Point"}, { toFieldInfo(p.x_).retag(Tag{"x"}), toFieldInfo(p.y_).retag(Tag{"y"}) } };
}

struct Line
{
  Point from_;
  Point to_;
};

auto toFieldInfo(Line const& l)
{
  using But::Log::Backend::toFieldInfo;
  return FieldInfo{ Tag{"Line"}, { toFieldInfo(l.from_).retag(Tag{"from"}), toFieldInfo(l.to_).retag(Tag{"to"}) } };
}


auto makeLogger(std::string host, const uint16_t port)
{
  auto tcp = But::makeSharedNN<But::Log::Destination::JsonTcp>( std::move(host), port );
  auto console = But::makeSharedNN<But::Log::Destination::TextConsole>();
  auto multi = But::makeSharedNN<But::Log::Destination::MultiSink>( std::move(console), std::move(tcp) );
  auto bgThread = But::makeSharedNN<But::Log::Destination::BackgroundThread>( std::move(multi), 2*1000 );
  But::Log::Proxy<> p{ std::move(bgThread) };
  return p;
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

  auto p = makeLogger( argv[1], boost::lexical_cast<uint16_t>(argv[2]) );
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
        p.log( args2FieldInfo( Timestamp{}, Priority::info, '@', FileName{__FILE__}, ':', LineNumber{__LINE__}, Component{"zerowy"}, "hello, world" ) );
        break;
      case 1:
        p.log( args2FieldInfo( Timestamp{}, Priority::debug, Component{"point-hair boss"}, Point{13, 42} ) );
        break;
      case 2:
        p.log( args2FieldInfo( Timestamp{}, Priority::debug, Component{"multi-point"}, Point{69, 997}, Point{13, 42} ) );
        break;
      case 3:
        p.log( args2FieldInfo( Priority::warning, Component{"line man"}, Line{ Point{1, 2}, Point{3, 4} } ) );
        break;
    }

    if( Clock::now() >= flushDeadline )
    {
      p.flush();
      flushDeadline += flushDelay;
    }

    std::this_thread::sleep_until(deadline);

    // NOTE: you can also play with:
    // p.reload();
    // to simulate dosconnections every now and then...
  }
}
