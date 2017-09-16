#pragma once
#include <json.hpp>
#include "But/Log/Backend/Entry.hpp"

namespace But
{
namespace Log
{
namespace Destination
{
namespace Common
{

nlohmann::json toJsonField(Backend::FieldInfo const& fi);

void toJson(nlohmann::json& out, Backend::Entry const& entry);

nlohmann::json toJson(Backend::Entry const& entry);

}
}
}
}
