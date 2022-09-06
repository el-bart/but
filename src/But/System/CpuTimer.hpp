#pragma once
#include <optional>
#include <x86intrin.h>
#include <cassert>

namespace But::System
{

struct CpuTimer final
{
  CpuTimer(): start_{ __rdtscp(&cpuNo_) } { }

  std::optional<uint64_t> elapsed() const
  {
    unsigned int cpuNo;
    const auto stop = __rdtscp(&cpuNo);
    if( cpuNo != cpuNo_ )
      return {};
    assert( stop >= start_ );
    return stop-start_;
  }

private:
  unsigned int cpuNo_;
  uint64_t start_;
};

}
