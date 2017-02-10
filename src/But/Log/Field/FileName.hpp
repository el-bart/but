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


inline std::string toString(FileName const& fn) { return fn.value_.string(); }

}
}
}
