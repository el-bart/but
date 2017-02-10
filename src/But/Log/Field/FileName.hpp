#pragma once
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


inline std::string typeString(FileName const&) { return "But::FileName"; }
inline std::string toString(FileName const& fn) { return fn.value_.string(); }

}
}
}
