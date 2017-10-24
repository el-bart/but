#include <sstream>
#include <iostream>
#include <But/Log/Backend/FieldInfo.hpp>
#include <But/Log/Backend/detail/StreamVisitor.hpp>

namespace But
{
namespace Log
{
namespace Backend
{

std::ostream& operator<<(std::ostream& os, FieldInfo const& fi)
{
  detail::StreamVisitor sv{&os};
  fi.visit(sv);
  return os;
}

std::string toString(FieldInfo const& fi)
{
  std::stringstream ss;
  ss << fi;
  return ss.str();
}

}
}
}
