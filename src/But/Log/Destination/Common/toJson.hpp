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

/** @brief returns JSON collection of top-level elements. internal elements are kept as JSON objects.
 */
nlohmann::json toJson(Backend::FieldInfo const& fi);

}
}
}
}
