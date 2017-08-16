#include <chrono>
#include <iostream>
#include "But/NotNull.hpp"
#include "LoggerProxy.hpp"
#include "Destination/Console.hpp"

using Clock = std::chrono::high_resolution_clock;

namespace
{

#if 1
constexpr auto g_logsCount = 10*1000*1000;
#else
constexpr auto g_logsCount = 100*1000;
#endif


auto makeLogger()
{
  auto dst = But::makeUniqueNN<But::Log::Destination::Console>();
  return But::Log::LoggerProxy<decltype(dst)>{ std::move(dst) };
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
    log.log("answer is ", 42);
}

template<typename Logger>
void testStringAndInts(Logger& log)
{
  for(auto i=0; i<g_logsCount; ++i)
    log.log("options: ", 41, 42, 43);
}

template<typename Logger>
void testStringAndDouble(Logger& log)
{
  for(auto i=0; i<g_logsCount; ++i)
    log.log("some value = ", 4.2);
}

}


int main(int argc, char** argv)
{
  if( argc != 1+1 )
  {
    std::cerr << argv[0] << "<test_case_number>" << std::endl;
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
    case 2: testStringAndInt(lp); break;
    case 3: testStringAndInts(lp); break;
    case 4: testStringAndDouble(lp); break;
    default: std::cerr << argv[0] << ": unknown test case " << testCaseNumber << std::endl; return 2;
  }
  const auto stop = Clock::now();
  const auto us = std::chrono::duration_cast<std::chrono::microseconds>( stop - start );
  std::cerr << argv[0] << ": done in " << std::fixed << us.count() / (1000.0*1000.0) << " [s]" << std::endl;

  lp.log("all done!");
  lp.flush();
}
