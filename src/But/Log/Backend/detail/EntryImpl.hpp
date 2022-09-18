#pragma once
#include <string>
#include <json.hpp> // implementation detail! do NOT rely on this!


namespace But::Log::Backend::detail
{

struct EntryImpl
{
  nlohmann::json json_;
};

}
