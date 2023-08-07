#pragma once
#include <nlohmann/json.hpp>
#include <string>

namespace But
{
namespace Log
{
namespace Destination
{
namespace Common
{

inline nlohmann::json toJson(std::string const& str)
{
  return nlohmann::json::parse(str);
}

}
}
}
}
