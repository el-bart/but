#include <boost/lexical_cast.hpp>
#include <json.hpp>
#include "JsonFile.hpp"

using json = nlohmann::json;

namespace But
{
namespace Log
{
namespace Destination
{

namespace
{
template<typename T>
auto convert(std::string const& str)
{
  return boost::lexical_cast<T>(str);
}

auto createField(Backend::FieldInfo const& fi)
{
  json field;

  // TODO: how about keeping value as a std::variant, instead of a string?

  if( fi.type() == "int" )
  {
    field[ std::move(fi).type() ] = convert<int64_t>( fi.value() );
    return field;
  }
  if( fi.type() == "unsigned int" )
  {
    field[ std::move(fi).type() ] = convert<uint64_t>( fi.value() );
    return field;
  }

  if( fi.type() == "double" )
  {
    field[ std::move(fi).type() ] = convert<double>( fi.value() );
    return field;
  }

  field[ std::move(fi).type() ] = std::move(fi).value();
  return field;
}

void commonFormat(json& tab, Backend::Entry const& entry)
{
  for(auto& e: entry)
    tab.push_back( createField(e) );
}
}


void JsonFile::toStreamFormat(std::ostream& os, Backend::Entry const& entry)
{
  auto tab = json::array();
  commonFormat(tab, entry);
  os << tab << "\n";
}


void JsonFile::toStreamFormat(std::ostream& os, Field::FormattedString const& str, Backend::Entry const& entry)
{
  auto tab = json::array();
  tab.push_back( createField( Backend::FieldInfo{str} ) );
  commonFormat(tab, entry);
  os << tab << "\n";
}

}
}
}
