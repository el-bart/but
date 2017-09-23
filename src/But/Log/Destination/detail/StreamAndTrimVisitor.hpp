#pragma once
#include <limits>
#include <iomanip>
#include <iostream>
#include <cassert>
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
  void operator()(const bool b)
  {
    assert(os_);
    (*os_) << std::boolalpha << b;
  }
  void operator()(const double fp)
  {
    assert(os_);
    (*os_) << std::fixed << std::setprecision( std::numeric_limits<long double>::digits10 + 1 ) << fp;
  }
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
