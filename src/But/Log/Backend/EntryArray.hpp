#pragma once
#include <string>
#include <cassert>
#include <cstdint>
#include <But/Log/Backend/detail/backendSelector.hpp>

namespace But::Log::Backend
{

struct EntryProxy;

struct EntryArray
{
  EntryArray() = delete;

  EntryArray(EntryArray const&) = default;
  EntryArray& operator=(EntryArray const&) = default;

  EntryArray(EntryArray&&) = default;
  EntryArray& operator=(EntryArray&&) = default;

  template<typename T>
  void nest(T const& t);

  template<typename C>
  void add(C const& c)
  {
    using std::begin;
    using std::end;
    add( begin(c), end(c) );
  }
  template<typename It>
  void add(It from, It to)
  {
    for(It it=from; it!=to; ++it)
      nest(*it);
  }

  EntryProxy object();
  EntryArray array();

  void value(); // null
  void value(bool v);
  void value(std::string_view v);
  void value(double v);
  void value(int64_t v);
  void value(uint64_t v);

  //
  // following overloads are only to make calls unabmiguous
  //
  // * integers
  void value(int32_t  v) { value(static_cast<int64_t >(v)); }
  void value(uint32_t v) { value(static_cast<uint64_t>(v)); }
  void value(int16_t  v) { value(static_cast<int64_t >(v)); }
  void value(uint16_t v) { value(static_cast<uint64_t>(v)); }
  void value(int8_t   v) { value(static_cast<int64_t >(v)); }
  void value(uint8_t  v) { value(static_cast<uint64_t>(v)); }
  // * floats
  void value(float v) { value(static_cast<double>(v)); }
  // * strings
  void value(char* v)            { value(std::string_view{v}); }
  void value(char const* v)      { value(std::string_view{v}); }
  template<size_t N>
  void value(char (&v)[N])       { value(std::string_view{v}); }
  template<size_t N>
  void value(char const (&v)[N]) { value(std::string_view{v}); }

private:
  friend struct EntryProxy;

  explicit EntryArray(void* impl): impl_{impl} { assert(impl_); }

  void* impl_;
};

}
