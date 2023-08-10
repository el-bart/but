#include <chrono>
#include <iostream>
#include <But/NotNull.hpp>
#include <But/Log/Logger.hpp>
#include <But/Log/Destination/Console.hpp>

using Clock = std::chrono::high_resolution_clock;
using But::Log::Backend::EntryProxy;

namespace
{

#if 1
constexpr auto g_logsCount = 10*1000*1000;
#else
constexpr auto g_logsCount = 100*1000;
#endif


auto makeLogger()
{
  auto dst = But::makeSharedNN<But::Log::Destination::Console>();
  return But::Log::Logger<>{ std::move(dst) };
}


void testCout()
{
  for(auto i=0; i<g_logsCount; ++i)
  {
    std::cout << "hello, world\n";
    //std::cout << "answer is " << 42 << "\n";
    //std::cout << "some value = " << 4.2 << "\n";
  }
}

template<typename Logger>
void testSimpleString(Logger& log)
{
  for(auto i=0; i<g_logsCount; ++i)
    log.log("hello, world");
}


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

template<typename Logger>
void testCustomStructure(Logger& log)
{
  for(auto i=0; i<g_logsCount; ++i)
    log.log( "agregated type", Point{-i, +i} );
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


template<typename Logger>
void testCustomNestedStructure(Logger& log)
{
  for(auto i=0; i<g_logsCount; ++i)
    log.log( "nested agregated type", Line{ Point{-i, +i}, Point{1,2} } );
}

}


int main(int argc, char** argv)
{
  if( argc != 1+1 )
  {
    std::cerr << argv[0] << " <test_case_number>" << std::endl;
    return 1;
  }
  const auto testCaseNumber = atoi(argv[1]);

  auto lp = makeLogger();
  lp.log("hello, log!");

  const auto start = Clock::now();
  switch(testCaseNumber)
  {
    case 0: testCout(); break;
    case 1: testSimpleString(lp); break;
    case 2: testCustomStructure(lp); break;
    case 3: testCustomNestedStructure(lp); break;
    default: std::cerr << argv[0] << ": unknown test case " << testCaseNumber << std::endl; return 2;
  }
  const auto stop = Clock::now();
  const auto us = std::chrono::duration_cast<std::chrono::microseconds>( stop - start );
  std::cerr << argv[0] << ": done in " << std::fixed << us.count() / (1000.0*1000.0) << " [s]" << std::endl;

  lp.log("all done!");
  lp.flush();
}
