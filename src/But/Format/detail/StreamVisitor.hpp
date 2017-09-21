#pragma once
#include <iostream>

namespace But
{
namespace Format
{
namespace detail
{

struct StreamVisitor
{
  template<typename T>
  void operator()(T const& t) const
  {
    (*os_) << t;
  }

  std::ostream* os_;
};

}
}
}
