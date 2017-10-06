#pragma once
#include <limits>
#include <iomanip>
#include <iostream>
#include <cassert>
#include "But/Log/Backend/FieldInfo.hpp"
#include "But/Log/Backend/NonPrintableTrimmer.hpp"
#include "But/Log/Field/FormattedString.hpp"

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
  void operator()(int64_t const& n)
  {
    assert(os_);
    (*os_) << n;
  }
  void operator()(uint64_t const& n)
  {
    assert(os_);
    (*os_) << n;
  }

  void operator()(Backend::Type const&, Backend::Value const& v)
  {
    v.visit(*this);
  }
  void operator()(Backend::Type const&, std::vector<Backend::FieldInfo> const& fis)
  {
    if( fis.empty() )
      return;
    if( fis[0].type() == formattedStringType_ )
    {
      assert(os_);
      (*os_) << fis[0].value().get<std::string>();
      return;
    }

    auto it = begin(fis);
    it->visit(*this);
    ++it;
    for(; it!=end(fis); ++it)
    {
      assert(os_);
      (*os_) << ' ';
      it->visit(*this);
    }
  }

  Backend::NonPrintableTrimmer const* trim_{nullptr};
  std::ostream* os_{nullptr};
  const Backend::Type formattedStringType_{ toFieldInfo( Field::FormattedString{""} ).type() };
};

}
}
}
}
