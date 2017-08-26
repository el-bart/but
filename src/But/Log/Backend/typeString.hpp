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

// it is quite common case, yet it outputs implementation details, instead of a human-readable typedef.
inline std::string typeString(std::string const&)  { return "std::string"; }
inline std::string typeString(std::wstring const&) { return "std::wstring"; }

// all CV-qualified char* types are considered just string, for the sake of simplicity
inline std::string typeString(char                *) { return "std::string"; }
inline std::string typeString(char const          *) { return "std::string"; }
inline std::string typeString(char       volatile *) { return "std::string"; }
inline std::string typeString(char const volatile *) { return "std::string"; }

// all signed numbers to be kept as just "int"
inline std::string typeString(char)      { return "int"; }
inline std::string typeString(short)     { return "int"; }
inline std::string typeString(int)       { return "int"; }
inline std::string typeString(long)      { return "int"; }
inline std::string typeString(long long) { return "int"; }

// all unsigned numbers to be kept as just "unsigned int"
inline std::string typeString(unsigned char)      { return "unsigned int"; }
inline std::string typeString(unsigned short)     { return "unsigned int"; }
inline std::string typeString(unsigned int)       { return "unsigned int"; }
inline std::string typeString(unsigned long)      { return "unsigned int"; }
inline std::string typeString(unsigned long long) { return "unsigned int"; }

// all fps are just "double"
inline std::string typeString(float)       { return "double"; }
inline std::string typeString(double)      { return "double"; }
inline std::string typeString(long double) { return "double"; }

}
}
}
