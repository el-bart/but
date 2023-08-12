#pragma once
#include <But/Log/Backend/EntryRoot.hpp>
#include <nlohmann/json.hpp>

namespace But::Log::Backend::detail
{

inline std::string unifyJson(nlohmann::json const& in)
{
  return in.dump(4);
}

inline std::string unifyJson(std::string const& in)
{
  return unifyJson( nlohmann::json::parse(in) );
}

inline std::string unifyJson(char const* in)
{
  return unifyJson( nlohmann::json::parse(in) );
}

inline std::string unifyJson(But::Log::Backend::EntryRoot const& in)
{
  return unifyJson( in.json() );
}

}
