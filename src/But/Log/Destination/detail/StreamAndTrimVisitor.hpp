#pragma once
#include <limits>
#include <iomanip>
#include <iostream>
#include "But/assert.hpp"
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
    BUT_ASSERT(os_);
    (*os_) << std::boolalpha << b;
  }
  void operator()(const double fp)
  {
    BUT_ASSERT(os_);
    (*os_) << std::fixed << std::setprecision( std::numeric_limits<long double>::digits10 + 1 ) << fp;
  }
  void operator()(std::string str)
  {
    BUT_ASSERT(os_);
    BUT_ASSERT(trim_);
    (*os_) << (*trim_)( std::move(str) );
  }
  void operator()(int64_t const& n)
  {
    BUT_ASSERT(os_);
    (*os_) << n;
  }
  void operator()(uint64_t const& n)
  {
    BUT_ASSERT(os_);
    (*os_) << n;
  }

  void operator()(Backend::Tag const&, Backend::Value const& v)
  {
    v.visit(*this);
  }
  void operator()(Backend::Tag const& t, std::vector<Backend::FieldInfo> const& fis)
  {
    if( fis.empty() )
      return;

    const auto isRoot = not rootProcessed_;
    rootProcessed_ = true;

    if( isRoot && fis[0].tag() == formattedStringType_ )
    {
      BUT_ASSERT(os_);
      (*os_) << fis[0].value().get<std::string>();
      return;
    }

    if(not isRoot)
    {
      BUT_ASSERT(os_);
      (*os_) << t << "={";
    }
    {
      auto it = begin(fis);
      it->visit(*this);
      ++it;
      for(; it!=end(fis); ++it)
      {
        BUT_ASSERT(os_);
        (*os_) << (isRoot?' ':',');
        it->visit(*this);
      }
    }
    if(not isRoot)
      (*os_) << "}";
  }

  Backend::NonPrintableTrimmer const* trim_{nullptr};
  std::ostream* os_{nullptr};
  const Backend::Tag formattedStringType_{ toFieldInfo( Field::FormattedString{""} ).tag() };
  bool rootProcessed_{false};
};

}
}
}
}
