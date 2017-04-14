#include <boost/lexical_cast.hpp>
#include "ThreadId.hpp"

namespace But
{
namespace Log
{
namespace Field
{

std::string toString(ThreadId const& tid)
{
  return boost::lexical_cast<std::string>(tid.value_);
}

}
}
}
