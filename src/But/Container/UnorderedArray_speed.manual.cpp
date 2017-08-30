#include <set>
#include <list>
#include <deque>
#include <chrono>
#include <string>
#include <random>
#include <vector>
#include <iomanip>
#include <typeinfo>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <cinttypes>
#include "But/assert.hpp"
#include "UnorderedArray.hpp"

using std::cout;
using std::cerr;
using std::endl;

using DataField = int;

struct UnorderedArrayAdapter
{
  using Container = But::Container::UnorderedArray<DataField>;
  void add(DataField i) { c_.add(i); }
  Container::const_iterator find(const DataField q) { return std::find( begin(c_), end(c_), q ); }
  void reserve(const Container::size_type n) { c_.reserve(n); }

  Container c_;
};


struct VectorAdapter
{
  using Container = std::vector<DataField>;
  void add(DataField i) { c_.push_back(i); }
  Container::const_iterator find(const DataField q) { return std::find( begin(c_), end(c_), q ); }
  void reserve(const Container::size_type n) { c_.reserve(n); }

  Container c_;
};


struct SetAdapter
{
  using Container = std::set<DataField>;
  void add(DataField i) { c_.insert(i); }
  Container::const_iterator find(const DataField q) { return c_.find(q); }
  void reserve(const Container::size_type /*n*/) { /* not applicable */ }

  Container c_;
};


struct UnorderedSetAdapter
{
  using Container = std::unordered_set<DataField>;
  void add(DataField i) { c_.insert(i); }
  Container::const_iterator find(const DataField q) { return c_.find(q); }
  void reserve(const Container::size_type n) { c_.reserve(n); }

  Container c_;
};


struct DequeAdapter
{
  using Container = std::deque<DataField>;
  void add(DataField i) { c_.push_back(i); }
  Container::const_iterator find(const DataField q) { return std::find( begin(c_), end(c_), q ); }
  void reserve(const Container::size_type /*n*/) { /* not applicable */ }

  Container c_;
};


struct ListAdapter
{
  using Container = std::list<DataField>;
  void add(DataField i) { c_.push_back(i); }
  Container::const_iterator find(const DataField q) { return std::find( begin(c_), end(c_), q ); }
  void reserve(const Container::size_type /*n*/) { /* not applicable */ }

  Container c_;
};


auto randomize(std::vector<DataField> c, const DataField seed)
{
  std::mt19937 gen(seed);
  std::shuffle( begin(c), end(c), gen );
  return c;
}


auto generateUniqueValues(unsigned size)
{
  const auto seed = 42;
  std::mt19937 gen(seed);
  std::uniform_int_distribution<DataField> dist;

  std::vector<DataField> out;
  out.reserve(size);
  for(auto i=0u; i<size; ++i)
  {
    while(true)
    {
      const auto next = dist(gen);
      const auto it   = std::lower_bound( begin(out), end(out), next );
      if(*it == next)
        continue;
      out.insert(it, next);
      break;
    }
  }
  assert( out.size() == size );
  assert( std::is_sorted( begin(out), end(out) ) );

  return randomize( std::move(out), seed+1 );
}


template<typename Adapter>
void testAlgorithm(std::vector<DataField> const& values, std::vector<DataField> const& queries)
{
  Adapter data;
  data.reserve( values.size() );
  for(const auto e: values)
    data.add(e);

  for(const auto q: queries)
  {
    const auto it = data.find(q);
    assert( it != end(data.c_) );
    data.c_.erase(it);
  }
}


template<typename Adapter>
void measure(std::vector<DataField> const& values, std::vector<DataField> const& queries)
{
  cerr << "measuring " << typeid(Adapter).name() << "..." << endl;
  using Clock = std::chrono::high_resolution_clock;
  const auto start = Clock::now();
  testAlgorithm<Adapter>(values, queries);
  const auto stop  = Clock::now();
  const auto diff  = stop - start;
  cerr.width(10);
  cerr.fill(' ');
  cerr << std::chrono::duration_cast<std::chrono::microseconds>(diff).count() << u8"[µs]" << endl;
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

    cerr << "preparing data..." << endl;
    const auto max = std::stoul(argv[1]);
    const auto values = generateUniqueValues(max);
    const auto queries = randomize(values, 13);

    cerr << "testing..." << endl;
    measure<UnorderedArrayAdapter>(values, queries);
    measure<VectorAdapter>(values, queries);
    measure<SetAdapter>(values, queries);
    measure<UnorderedSetAdapter>(values, queries);
    measure<DequeAdapter>(values, queries);
    measure<ListAdapter>(values, queries);
  }
  catch(std::exception const& ex)
  {
    cerr << argv[0] << " ERROR: " << ex.what() << endl;
    return 2;
  }
}
