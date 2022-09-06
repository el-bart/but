#pragma once
#include <But/Threading/ThreadPool.hpp>

namespace But
{
namespace Threading
{

class ActiveObject final
{
public:
  ActiveObject() = default;

  template<typename F>
  auto run(F f)
  {
    return tp_.run( std::move(f) );
  }

private:
  ThreadPool tp_{ ThreadsCount{1} };
};

}
}
