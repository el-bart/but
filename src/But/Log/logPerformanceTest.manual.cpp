#include <chrono>
#include <iostream>
#include "But/NotNull.hpp"
#include "Proxy.hpp"
#include "Destination/TextConsole.hpp"

using Clock = std::chrono::high_resolution_clock;
using But::Log::Backend::Type;
using But::Log::Backend::FieldInfo;

namespace
{

#if 1
constexpr auto g_logsCount = 10*1000*1000;
#else
constexpr auto g_logsCount = 100*1000;
#endif


auto makeLogger()
{
  auto dst = But::makeUniqueNN<But::Log::Destination::TextConsole>();
  return But::Log::Proxy<decltype(dst)>{ std::move(dst) };
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

template<typename Logger>
void testStringAndInt(Logger& log)
{
  for(auto i=0; i<g_logsCount; ++i)
    log.log("answer is", 42);
}

template<typename Logger>
void testStringAndInts(Logger& log)
{
  for(auto i=0; i<g_logsCount; ++i)
    log.log("options:", 41, 42, 43);
}

template<typename Logger>
void testStringAndDouble(Logger& log)
{
  for(auto i=0; i<g_logsCount; ++i)
    log.log("some value =", 4.2);
}


struct Point
{
  int x_;
  int y_;
};

auto toFieldInfo(Point const& p)
{
  using But::Log::Backend::toFieldInfo;
  return FieldInfo{ Type{"Point"}, { toFieldInfo(p.x_), toFieldInfo(p.y_) } };
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

auto toFieldInfo(Line const& l)
{
  using But::Log::Backend::toFieldInfo;
  return FieldInfo{ Type{"Line"}, { toFieldInfo(l.from_), toFieldInfo(l.to_) } };
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
    case 2: testSimpleString(lp); break;
    case 3: testStringAndInt(lp); break;
    case 4: testStringAndInts(lp); break;
    case 5: testStringAndDouble(lp); break;
    case 6: testCustomStructure(lp); break;
    case 7: testCustomNestedStructure(lp); break;
    default: std::cerr << argv[0] << ": unknown test case " << testCaseNumber << std::endl; return 2;
  }
  const auto stop = Clock::now();
  const auto us = std::chrono::duration_cast<std::chrono::microseconds>( stop - start );
  std::cerr << argv[0] << ": done in " << std::fixed << us.count() / (1000.0*1000.0) << " [s]" << std::endl;

  lp.log("all done!");
  lp.flush();
}
