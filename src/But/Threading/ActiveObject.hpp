#pragma once
#include "ThreadPool.hpp"
#include "ThreadPoolStdPolicy.hpp"

namespace But
{
namespace Threading
{

template<typename Policy>
using GenericActiveObject = ThreadPool<Policy>;

using ActiveObject = GenericActiveObject<ThreadPoolStdPolicy>;

}
}
