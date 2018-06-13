#include <string>
#include <But/Log/Field/ConversionError.hpp>

namespace But
{
namespace Log
{
namespace Field
{
namespace detail
{

std::string date2str(const time_t timestamp);
std::string time2str(const time_t timestamp);

}
}
}
}
