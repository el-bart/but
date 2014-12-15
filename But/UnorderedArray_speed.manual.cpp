#include <vector>
#include "UnorderedArray.hpp"

#if 1
struct Adapter
{
  using Container = But::UnorderedArray<int>;
  void add(int i) { c_.add(i); }

  Container c_;
};
#else
struct Adapter
{
  using Container = std::vector<int>;
  void add(int i) { c_.push_back(i); }

  Container c_;
};
#endif

int main()
{
  Adapter data;

  constexpr auto max = 100*1000;
  data.c_.reserve(max);
  for(auto i=0; i<max; ++i)
    data.add(i);

  while( not data.c_.empty() )
    data.c_.erase( std::begin(data.c_) + data.c_.size()/2u );
}
