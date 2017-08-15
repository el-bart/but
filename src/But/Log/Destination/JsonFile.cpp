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

auto createField(Backend::FieldInfo&& fi)
{
  json field;

  // TODO: optimize this...

  if( fi.type() == "short" )
  {
    field[ std::move(fi).type() ] = convert<short>( fi.value() );
    return field;
  }
  if( fi.type() == "unsigned short" )
  {
    field[ std::move(fi).type() ] = convert<unsigned short>( fi.value() );
    return field;
  }

  if( fi.type() == "int" )
  {
    field[ std::move(fi).type() ] = convert<int>( fi.value() );
    return field;
  }
  if( fi.type() == "unsigned int" )
  {
    field[ std::move(fi).type() ] = convert<unsigned int>( fi.value() );
    return field;
  }

  if( fi.type() == "long" )
  {
    field[ std::move(fi).type() ] = convert<long>( fi.value() );
    return field;
  }
  if( fi.type() == "unsigned long" )
  {
    field[ std::move(fi).type() ] = convert<unsigned long>( fi.value() );
    return field;
  }

  if( fi.type() == "long long" )
  {
    field[ std::move(fi).type() ] = convert<long long>( fi.value() );
    return field;
  }
  if( fi.type() == "unsigned long long" )
  {
    field[ std::move(fi).type() ] = convert<unsigned long long>( fi.value() );
    return field;
  }

  if( fi.type() == "float" )
  {
    field[ std::move(fi).type() ] = convert<float>( fi.value() );
    return field;
  }
  if( fi.type() == "double" )
  {
    field[ std::move(fi).type() ] = convert<double>( fi.value() );
    return field;
  }
  if( fi.type() == "long double" )
  {
    field[ std::move(fi).type() ] = convert<long double>( fi.value() );
    return field;
  }

  field[ std::move(fi).type() ] = std::move(fi).value();
  return field;
}
}

void JsonFile::toStreamFormat(std::ostream& os, Backend::Entry&& entry)
{
  // TODO: format field - special case

  auto tab = json::array();
  for(auto& e: entry)
    tab.push_back( createField( std::move(e) ) );
  os << tab << "\n";
}

}
}
}
