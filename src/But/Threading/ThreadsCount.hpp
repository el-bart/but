#pragma once
#include <But/Exception.hpp>

namespace But
{
namespace Threading
{

class ThreadsCount final
{
public:
  BUT_DEFINE_EXCEPTION(InvalidNumberOfThreads, Exception, "invalid number of threads");

  using value_type = size_t;

  explicit ThreadsCount(value_type count):
    count_{count}
  {
    if(count_ < 1u)
      BUT_THROW(InvalidNumberOfThreads, "must be at least 1");
  }

  value_type count() const { return count_; }

private:
  value_type count_;
};

}
}
