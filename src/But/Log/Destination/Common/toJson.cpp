#include <boost/lexical_cast.hpp>
#include "toJson.hpp"

using json = nlohmann::json;

namespace But
{
namespace Log
{
namespace Destination
{
namespace Common
{

namespace
{
template<typename T>
auto convert(std::string const& str)
{
  return boost::lexical_cast<T>(str);
}
}


nlohmann::json toJsonField(Backend::FieldInfo const& fi)
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


void toJson(json& out, Backend::Entry const& entry)
{
  for(auto& e: entry)
    out.push_back( toJsonField(e) );
}


nlohmann::json toJson(Backend::Entry const& entry)
{
  auto out = nlohmann::json::array();
  toJson(out, entry);
  return out;
}

}
}
}
}
