#pragma once
#include <string>
#include <cassert>
#include <cstdint>
#include <But/Log/Backend/EntryArray.hpp>
#include <But/Log/Backend/detail/backendSelector.hpp>

namespace But::Log::Backend
{

struct EntryRoot;

struct EntryProxy
{
  EntryProxy() = delete;

  EntryProxy(EntryProxy const&) = default;
  EntryProxy& operator=(EntryProxy const&) = default;

  EntryProxy(EntryProxy&&) = default;
  EntryProxy& operator=(EntryProxy&&) = default;

  // TODO: finish implemenattion, based on SFINAE
  template<typename T>
  void nest(T const& t)
  {
    auto name = fieldName( static_cast<T*>(nullptr) );
    if constexpr ( detail::HasObjectValue<T>::value )
    {
      auto p = object( std::move(name) );
      objectValue(p, t);
    }
    else
      if constexpr ( detail::HasFieldValue<T>::value )
        value( std::move(name), fieldValue(t) );
      else
        if constexpr ( detail::HasArrayValue<T>::value )
        {
          auto a = array( std::move(name) );
          arrayValue(a, t);
        }
        else
          static_assert(!"cannot nest type - missing coversion function" && sizeof(T));
  }

  /*
  template<typename=hasFieldValue...>
  void nest(auto& obj);
  template<typename=hasFieldObject...>
  void nest(auto& obj);
  template<typename=hasFieldArray...>
  void nest(auto& obj);
  */

  EntryProxy object(std::string_view name);
  EntryArray array(std::string_view name);

  void value(std::string_view name); // null
  void value(std::string_view name, bool v);
  void value(std::string_view name, std::string_view v);
  void value(std::string_view name, double v);
  void value(std::string_view name, int64_t v);
  void value(std::string_view name, uint64_t v);

  //
  // following overloads are only to make calls unabmiguous
  //
  // * integers
  void value(std::string_view name, int32_t  v) { value(name, static_cast<int64_t >(v)); }
  void value(std::string_view name, uint32_t v) { value(name, static_cast<uint64_t>(v)); }
  void value(std::string_view name, int16_t  v) { value(name, static_cast<int64_t >(v)); }
  void value(std::string_view name, uint16_t v) { value(name, static_cast<uint64_t>(v)); }
  void value(std::string_view name, int8_t   v) { value(name, static_cast<int64_t >(v)); }
  void value(std::string_view name, uint8_t  v) { value(name, static_cast<uint64_t>(v)); }
  // * floats
  void value(std::string_view name, float v) { value(name, static_cast<double>(v)); }
  // * strings
  void value(std::string_view name, char* v)            { value(name, std::string_view{v}); }
  void value(std::string_view name, char const* v)      { value(name, std::string_view{v}); }
  template<size_t N>
  void value(std::string_view name, char (&v)[N])       { value(name, std::string_view{v}); }
  template<size_t N>
  void value(std::string_view name, char const (&v)[N]) { value(name, std::string_view{v}); }

  // TODO: adding elements goes here

private:
  friend struct EntryRoot;
  friend struct EntryArray;

  explicit EntryProxy(void* impl): impl_{impl} { assert(impl_); }

  void* impl_;
};

}

#include <But/Log/Backend/EntryArray_impl.hpp>
