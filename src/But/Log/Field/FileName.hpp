#pragma once
#include <boost/filesystem/path.hpp>
#include "But/Log/Backend/FieldInfo.hpp"

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


inline auto toFieldInfo(FileName const& fn)
{
  return Backend::FieldInfo{ Backend::Type{"But::FileName"}, Backend::Value{ fn.value_.string() } };
}

}
}
}
