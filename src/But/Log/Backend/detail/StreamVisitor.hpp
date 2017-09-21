#pragma once
#include <iostream>
#include <cassert>

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
    assert(os_);
    (*os_) << t;
  }

  std::ostream* os_;
};

}
}
}
}
