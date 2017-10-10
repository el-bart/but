#include <type_traits>
#include <stdexcept>
#include "But/assert.hpp"
#include "Priority.hpp"


namespace But
{
namespace Log
{
namespace Field
{

namespace
{
Backend::Value const& toValue(const Priority pri)
{
  static const Backend::Value out[] =
                    {
                      Backend::Value{"debug"},
                      Backend::Value{"info"},
                      Backend::Value{"warning"},
                      Backend::Value{"error"}
                    };
  const auto num = static_cast<unsigned>(pri);
  BUT_ASSERT( num < std::extent<decltype(out)>::value && "unknown priority" );
  return out[num];
}
}

Backend::FieldInfo toFieldInfo(const Priority pri)
{
  return Backend::FieldInfo{ Backend::Tag{"But::Priority"}, toValue(pri) };
}

}
}
}
