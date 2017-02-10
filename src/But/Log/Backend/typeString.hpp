/** @brief customization point for converting user-defined values to a printable string.
 *         ADL is used to search for proper overload. default one will print using boost's pretty-printing.
 */
#pragma once
#include <string>
#include <boost/type_index.hpp>

namespace But
{
namespace Log
{
namespace Backend
{

/** @brief by default just print using boost::type_index.
 */
template<typename T>
std::string typeString(T const&) { return boost::typeindex::type_id<T>().pretty_name(); }

/** @brief it is quite common case, yet it outputs implementation details, instead of a human-readable typedef.
 */
inline std::string typeString(std::string const&) { return "std::string"; }

}
}
}
