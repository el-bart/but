#pragma once
#include "ThreadPool.hpp"
#include "ThreadPoolStdPolicy.hpp"

namespace But
{
namespace Threading
{

template<typename Policy>
class GenericActiveObject final
{
public:
  GenericActiveObject() = default;

  template<typename F>
  auto run(F f)
  {
    return tp_.run( std::move(f) );
  }

private:
  using PoolType = ThreadPool<Policy>;
  PoolType tp_{ ThreadsCount{1} };
};


using ActiveObject = GenericActiveObject<ThreadPoolStdPolicy>;

}
}
