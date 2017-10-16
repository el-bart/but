#pragma once
#include <But/Threading/ThreadPool.hpp>

namespace But
{
namespace Threading
{

template<typename Policy>
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
  using PoolType = ThreadPool<Policy>;
  PoolType tp_{ ThreadsCount{1} };
};

}
}
