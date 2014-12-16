#include <chrono>
#include <string>
#include <random>
#include <vector>
#include <typeinfo>
#include <iostream>
#include <algorithm>
#include "UnorderedArray.hpp"

using std::cout;
using std::cerr;
using std::endl;


struct UnorderedArrayAdapter
{
  using Container = But::UnorderedArray<int>;
  void add(int i) { c_.add(i); }
  Container::const_iterator find(const int q) { return std::find( begin(c_), end(c_), q ); }

  Container c_;
};


struct VectorAdapter
{
  using Container = std::vector<int>;
  void add(int i) { c_.push_back(i); }
  Container::const_iterator find(const int q) { return std::find( begin(c_), end(c_), q ); }

  Container c_;
};


auto generateQueries(unsigned size)
{
  const auto seed = 42;
  std::mt19937 gen(seed);
  std::uniform_int_distribution<int> dist;

  std::vector<int> out;
  out.reserve(size);
  for(auto i=0u; i<size; ++i)
    out.push_back( dist(gen) );

  return out;
}


auto randomize(std::vector<int> c)
{
  const auto seed = 40+4;
  std::mt19937 gen(seed);
  std::shuffle( begin(c), end(c), gen );
  return c;
}


template<typename Adapter>
void testAlgorithm(std::vector<int> const& values, std::vector<int> const& queries)
{
  Adapter data;
  data.c_.reserve( values.size() );
  for(const auto e: values)
    data.add(e);


  for(const auto q: queries)
  {
    const auto it = std::find( begin(data.c_), end(data.c_), q );
    assert( it != end(data.c_) );
    data.c_.erase(it);
  }
}

template<typename Adapter>
void measure(std::vector<int> const& values, std::vector<int> const& queries)
{
  cerr << "measuring " << typeid(Adapter).name() << "...";
  using Clock = std::chrono::high_resolution_clock;
  const auto start = Clock::now();
  testAlgorithm<Adapter>(values, queries);
  const auto stop  = Clock::now();
  const auto diff  = stop - start;
  cout << " done in " << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << "[ms]" << endl;
}


int main(int argc, char** argv)
{
  try
  {
    if(argc!=1+1)
    {
      cerr << argv[0] << " <size>" << endl;
      return 1;
    }

    const auto max = std::stoul(argv[1]);
    const auto values = generateQueries(max);
    const auto queries = randomize(values);

    measure<UnorderedArrayAdapter>(values, queries);
    measure<VectorAdapter>(values, queries);
  }
  catch(std::exception const& ex)
  {
    cerr << argv[0] << " ERROR: " << ex.what() << endl;
    return 2;
  }
}
