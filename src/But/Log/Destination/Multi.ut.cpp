#include "gtest/gtest.h"
#include "Multi.hpp"

using But::Log::Destination::Multi;

namespace
{

template<int N>
struct TestDst final
{
  template<typename ...Args>
  void log(Args&& ...)
  {
    ++calls_;
    if(throws_)
      throw std::runtime_error{"throwing, as requested"};
  }

  void reload()
  {
    ++reloads_;
    if(throws_)
      throw std::runtime_error{"throwing, as requested"};
  }

  auto operator->() { return this; }

  bool throws_{false};
  unsigned calls_{0};
  unsigned reloads_{0};
};


struct ButLogDestinationMulti: public testing::Test
{
  TestDst<1> td1_;
  TestDst<2> td2_;
  TestDst<3> td3_;
  Multi<TestDst<1>, TestDst<2>, TestDst<3>> multi_;
};


TEST_F(ButLogDestinationMulti, SingleDestinationWorks)
{
}


TEST_F(ButLogDestinationMulti, PrintingGoesThroughAllDestinations)
{
}


TEST_F(ButLogDestinationMulti, ExceptionInAnyPrinterDoesNotStopProcessing)
{
}


TEST_F(ButLogDestinationMulti, ReloadingReloadsAllDestinations)
{
}


TEST_F(ButLogDestinationMulti, ExceptionInAnyDestinationDoesNotStopProcessing)
{
}

}
