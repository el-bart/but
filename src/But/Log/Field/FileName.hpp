#pragma once
#include <boost/filesystem/path.hpp>
#include "But/Log/Backend/Type.hpp"
#include "But/Log/Backend/Value.hpp"

namespace But
{
namespace Log
{
namespace Field
{

struct FileName final
{
  boost::filesystem::path value_;
};


inline auto toType(FileName const&) { return Backend::Type{"But::FileName"}; }
inline auto toValue(FileName const& fn) { return Backend::Value{ fn.value_.string() }; }

}
}
}
