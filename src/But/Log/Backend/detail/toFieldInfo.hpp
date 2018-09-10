#pragma once
#include <tuple>
#include <vector>
#include <utility>

namespace But
{
namespace Log
{
namespace Backend
{

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
  return FieldInfo{ Tag{"tuple"}, std::move(out) };
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
