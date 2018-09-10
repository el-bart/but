#pragma once
#include <tuple>
#include <vector>
#include <utility>
#include <But/Log/Backend/FieldInfo.hpp>

namespace But
{
namespace Log
{
namespace Backend
{

// identity
inline auto  toFieldInfo(FieldInfo&& fi) { return std::move(fi); }
inline auto& toFieldInfo(FieldInfo const& fi) { return fi; }

// boolean
inline auto toFieldInfo(bool v) { return FieldInfo{ Tag{"bool"}, Value{v} }; }

// it is quite common case, yet it outputs implementation details, instead of a human-readable typedef.
inline auto toFieldInfo(std::string v)  { return FieldInfo{ Tag{"string"}, Value{ std::move(v) } }; }
//inline auto toFieldInfo(std::wstring v) { return FieldInfo{ Tag{"string"}, Value{ std::move(v) } }; }

// all CV-qualified char* types are considered just string, for the sake of simplicity
inline auto toFieldInfo(char               * v) { return FieldInfo{ Tag{"string"}, Value{v} }; }
inline auto toFieldInfo(char const         * v) { return FieldInfo{ Tag{"string"}, Value{v} }; }
inline auto toFieldInfo(char       volatile* v) { return FieldInfo{ Tag{"string"}, Value{v} }; }
inline auto toFieldInfo(char const volatile* v) { return FieldInfo{ Tag{"string"}, Value{v} }; }

// all signed numbers to be kept as just "int"
inline auto toFieldInfo(char v)      { return FieldInfo{ Tag{"string"}, Value{v} }; }
inline auto toFieldInfo(short v)     { return FieldInfo{ Tag{"int"}, Value{v} }; }
inline auto toFieldInfo(int v)       { return FieldInfo{ Tag{"int"}, Value{v} }; }
inline auto toFieldInfo(long v)      { return FieldInfo{ Tag{"int"}, Value{v} }; }
inline auto toFieldInfo(long long v) { return FieldInfo{ Tag{"int"}, Value{v} }; }

// all unsigned numbers to be kept as just "unsigned int"
inline auto toFieldInfo(unsigned char v)      { return FieldInfo{ Tag{"unsigned int"}, Value{v} }; }
inline auto toFieldInfo(unsigned short v)     { return FieldInfo{ Tag{"unsigned int"}, Value{v} }; }
inline auto toFieldInfo(unsigned int v)       { return FieldInfo{ Tag{"unsigned int"}, Value{v} }; }
inline auto toFieldInfo(unsigned long v)      { return FieldInfo{ Tag{"unsigned int"}, Value{v} }; }
inline auto toFieldInfo(unsigned long long v) { return FieldInfo{ Tag{"unsigned int"}, Value{v} }; }

// all fps are just "double"
inline auto toFieldInfo(float v)       { return FieldInfo{ Tag{"double"}, Value{v} }; }
inline auto toFieldInfo(double v)      { return FieldInfo{ Tag{"double"}, Value{v} }; }
inline auto toFieldInfo(long double v) { return FieldInfo{ Tag{"double"}, Value{v} }; }

FieldInfo toFieldInfo(std::exception const& ex);

template<typename T>
inline auto toFieldInfo(std::vector<T> v)
{
  std::vector<FieldInfo> out;
  out.reserve( v.size() );
  for(auto& e: v)
    out.push_back( toFieldInfo( std::move(e) ) );
  return FieldInfo{ Tag{"sequence"}, std::move(out) };
}

template<typename F, typename S>
inline auto toFieldInfo(std::pair<F,S> v)
{
  std::vector<FieldInfo> out;
  out.reserve(2);
  out.push_back( toFieldInfo( std::move(v.first)  ) );
  out.push_back( toFieldInfo( std::move(v.second) ) );
  return FieldInfo{ Tag{"pair"}, std::move(out) };
}

namespace detail
{
template<typename ...Args, std::size_t ...Index>
inline auto tuple2Vector(std::tuple<Args...> t, std::index_sequence<Index...>)
{
  std::vector<FieldInfo> out{ toFieldInfo( std::get<Index>(t) )... };
  return out;
}
}

template<typename ...Args>
inline auto toFieldInfo(std::tuple<Args...> t)
{
  auto out = detail::tuple2Vector( std::move(t), std::index_sequence_for<Args...>{} );
  return FieldInfo{ Tag{"tuple"}, std::move(out) };
}

}
}
}
