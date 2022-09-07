#pragma once
#include <filesystem>
#include <But/Log/Backend/FieldInfo.hpp>

namespace But
{
namespace Log
{
namespace Field
{

struct FileName final
{
  std::filesystem::path value_;
};


inline auto toFieldInfo(FileName const& fn)
{
  return Backend::FieldInfo{ Backend::Tag{"But::FileName"}, Backend::Value{ fn.value_.string() } };
}

}
}
}
