#pragma once
#include <string>
#include <boost/filesystem/path.hpp>
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


inline std::string toType(FileName const&) { return "But::FileName"; }
inline Backend::Value toValue(FileName const& fn) { return Backend::Value{ fn.value_.string() }; }

}
}
}
