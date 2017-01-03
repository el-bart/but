#pragma once
#include <vector>

namespace But
{
namespace Container
{

template<typename T, size_t N, typename Container = std::vector<T>>
class MaxSizedSequence final
{
public:
  MaxSizedSequence() = default;
  // TODO

private:
  Container c_;
  size_t    additions_;
};

}
}
