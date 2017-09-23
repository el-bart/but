#pragma once
#include <cassert>
#include <iostream>
#include "But/Log/Backend/NonPrintableTrimmer.hpp"

namespace But
{
namespace Log
{
namespace Destination
{
namespace detail
{

struct StreamAndTrimVisitor final
{
  void operator()(std::string str)
  {
    assert(os_);
    assert(trim_);
    (*os_) << (*trim_)( std::move(str) );
  }
  template<typename T>
  void operator()(T const& t)
  {
    assert(os_);
    (*os_) << t;
  }

  Backend::NonPrintableTrimmer const* trim_;
  std::ostream* os_;
};

}
}
}
}
