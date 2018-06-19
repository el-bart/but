#include <But/Log/Backend/toFieldInfo.hpp>
#include <boost/type_index.hpp>

namespace But
{
namespace Log
{
namespace Backend
{

FieldInfo toFieldInfo(std::exception const& ex)
{
  auto typeStr = boost::typeindex::type_id_runtime(ex).pretty_name();
  auto type = toFieldInfo( std::move(typeStr) ).retag( Tag{"Type"} );
  auto message = toFieldInfo( ex.what() ).retag( Tag{"Message"} );
  return FieldInfo{ Tag{"Exception"}, { std::move(type), std::move(message) } };
}


}
}
}
