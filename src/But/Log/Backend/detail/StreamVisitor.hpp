#pragma once
#include <iostream>
#include "But/assert.hpp"

namespace But
{
namespace Log
{
namespace Backend
{
namespace detail
{

struct StreamVisitor final
{
  template<typename T>
  void operator()(T const& t) const
  {
    BUT_ASSERT(os_);
    (*os_) << t;
  }

  std::ostream* os_;
};

}
}
}
}
