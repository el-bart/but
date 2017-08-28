#include <algorithm>
#include <cassert>
#include "Translation.hpp"

namespace But
{
namespace Log
{
namespace Localization
{

namespace
{
void ensureValidity(Translation::Data const& data)
{
  (void)data;
  //for(auto& e: data) // TODO...
}

auto sortData(Translation::Data&& data)
{
  ensureValidity(data);
  std::sort( begin(data), end(data) );
  return data;
}
}

Translation::Translation(Data data):
  data_{ makeSharedNN<const Data>( sortData( std::move(data) ) ) }
{ }


char const* Translation::findTranslation(char const* from) const
{
  assert( std::is_sorted( begin(*data_), end(*data_) ) );
  const auto it = std::lower_bound( begin(*data_), end(*data_), from );
  if( it == end(*data_) || it->from_.format_ != from )
    return nullptr;
  return it->to_.format_.c_str();
}


bool operator<(Translation::Entry const& lhs, Translation::Entry const& rhs) { return lhs.from_.format_ < rhs.from_.format_; }
bool operator<(char const* lhs,               Translation::Entry const& rhs) { return lhs               < rhs.from_.format_; }
bool operator<(Translation::Entry const& lhs, char const* rhs)               { return lhs.from_.format_ < rhs; }

}
}
}
