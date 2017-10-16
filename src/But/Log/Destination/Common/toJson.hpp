#pragma once
#include <json.hpp>
#include <But/Exception.hpp>
#include <But/Log/Backend/Entry.hpp>

namespace But
{
namespace Log
{
namespace Destination
{
namespace Common
{

BUT_DEFINE_EXCEPTION(RootElementMissing, But::Exception, "trying to serialize non-root element");

/** @brief returns JSON collection of top-level elements. internal elements are kept as JSON objects.
 *         field within a structure, with repeated names, get auto-numbered.
 */
nlohmann::json toJson(Backend::FieldInfo const& fi);

}
}
}
}
