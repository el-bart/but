#pragma once
#include <string>
#include <boost/filesystem/path.hpp>

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
inline std::string toValue(FileName const& fn) { return fn.value_.string(); }

}
}
}
