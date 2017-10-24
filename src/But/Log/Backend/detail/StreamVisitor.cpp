#include <limits>
#include <iomanip>
#include <iostream>
#include <But/assert.hpp>
#include <But/Log/Backend/FieldInfo.hpp>
#include <But/Log/Field/FormattedString.hpp>
#include <But/Log/Backend/detail/StreamVisitor.hpp>

namespace But
{
namespace Log
{
namespace Backend
{
namespace detail
{

void StreamVisitor::operator()(const bool b)
{
  BUT_ASSERT(os_);
  (*os_) << std::boolalpha << b;
}


void StreamVisitor::operator()(const double fp)
{
  BUT_ASSERT(os_);
  (*os_) << std::fixed << std::setprecision( std::numeric_limits<long double>::digits10 + 1 ) << fp;
}


void StreamVisitor::operator()(std::string const& str)
{
  BUT_ASSERT(os_);
  (*os_) << '"' << str << '"';
}


void StreamVisitor::operator()(const int64_t n)
{
  BUT_ASSERT(os_);
  (*os_) << n;
}


void StreamVisitor::operator()(const uint64_t n)
{
  BUT_ASSERT(os_);
  (*os_) << n;
}


void StreamVisitor::operator()(Backend::Tag const& t, Backend::Value const& v)
{
  BUT_ASSERT(os_);
  (*os_) << t << '=';
  v.visit(*this);
}


void StreamVisitor::operator()(Backend::Tag const& t, std::vector<Backend::FieldInfo> const& fis)
{
  if( fis.empty() )
    return;

  BUT_ASSERT(os_);

  (*os_) << t << "={";

  auto it = begin(fis);
  do
  {
    BUT_ASSERT(os_);
    if( it != begin(fis) )
      (*os_) << ',';
    it->visit(*this);
    ++it;
  }
  while( it != end(fis) );

  (*os_) << "}";
}

}
}
}
}
