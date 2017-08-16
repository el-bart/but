#include <chrono>
#include <iostream>
#include "But/NotNull.hpp"
#include "LoggerProxy.hpp"
#include "Destination/Console.hpp"

using Clock = std::chrono::high_resolution_clock;

namespace
{

auto makeLogger()
{
  auto dst = But::makeUniqueNN<But::Log::Destination::Console>();
  return But::Log::LoggerProxy<decltype(dst)>{ std::move(dst) };
}

template<typename Logger>
void testSimpleString(Logger& log)
{
  (void)log;        
  for(auto i=0; i<10*1000*1000; ++i)
    //log.log("hello, world");
    std::cout << "hello, world\n";
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
    case 0: testSimpleString(lp); break;
    default: std::cerr << argv[0] << ": unknown test case " << testCaseNumber << std::endl; return 2;
  }
  const auto stop = Clock::now();
  const auto us = std::chrono::duration_cast<std::chrono::microseconds>( stop - start );
  std::cerr << argv[0] << ": done in " << std::fixed << us.count() / (1000.0*1000.0) << " [s]" << std::endl;

  lp.log("all done!");
  lp.flush();
}
