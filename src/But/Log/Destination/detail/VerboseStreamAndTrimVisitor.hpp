#pragma once
#include <limits>
#include <iomanip>
#include <iostream>
#include <But/assert.hpp>
#include <But/Log/Backend/FieldInfo.hpp>
#include <But/Log/Backend/toFieldInfo.hpp>
#include <But/Log/Backend/NonPrintableTrimmer.hpp>
#include <But/Log/Field/FormattedString.hpp>

namespace But
{
namespace Log
{
namespace Destination
{
namespace detail
{

struct VerboseStreamAndTrimVisitor final
{
  struct DepthGuard
  {
    DepthGuard(bool isRoot, size_t depth):
      isRoot_{isRoot},
      depth_{depth}
    {
      if(not isRoot_)
        ++depth_;
    }
    ~DepthGuard()
    {
      if(not isRoot_)
        --depth_;
    }

  private:
    const bool isRoot_;
    size_t& depth_;
  };

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
    (*os_) << '"' << (*trim_)( std::move(str) ) << '"';
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

  void operator()(Backend::Tag const& t, Backend::Value const& v)
  {
    BUT_ASSERT(os_);
    (*os_) << t.str() << "=";
    v.visit(*this);
  }
  void operator()(Backend::Tag const& t, std::vector<Backend::FieldInfo> const& fis)
  {
    if( fis.empty() )
      return;

    const auto isRoot = not rootProcessed_;
    rootProcessed_ = true;

    if(not isRoot)
    {
      BUT_ASSERT(os_);
      (*os_) << t << "={ ";
    }

    if( currentDepth_ <= maxDepth_ )
      processAllNodes(isRoot, fis);
    else
      (*os_) << "...";

    if(not isRoot)
      (*os_) << " }";
  }

  void processAllNodes(const bool isRoot, std::vector<Backend::FieldInfo> const& fis)
  {
    const auto guard = DepthGuard{isRoot, currentDepth_};
    auto it = begin(fis);
    do
    {
      BUT_ASSERT(os_);
      if( it != begin(fis) )
        (*os_) << ' ';
      if( isRoot && it->tag() == formattedStringType_ )
      {
        BUT_ASSERT(trim_);
        (*os_) << (*trim_)( it->value().get<std::string>() );
        return;
      }
      if( isRoot && it->tag() == stringType_ )
      {
        BUT_ASSERT(trim_);
        (*os_) << (*trim_)( it->value().get<std::string>() );
      }
      else
        it->visit(*this);
      ++it;
    }
    while( it != end(fis) );
  }

  Backend::NonPrintableTrimmer const* trim_{nullptr};
  std::ostream* os_{nullptr};
  const size_t maxDepth_{0};

  const Backend::Tag formattedStringType_{ toFieldInfo( Field::FormattedString{""} ).tag() };
  const Backend::Tag stringType_{ Backend::toFieldInfo( std::string{} ).tag() };
  bool rootProcessed_{false};
  size_t currentDepth_{0};
};

}
}
}
}
