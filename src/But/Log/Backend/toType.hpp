/** @brief customization point for converting user-defined values to a printable string.
 *         ADL is used to search for proper overload.
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

// boolean
inline std::string toType(bool const&)  { return "bool"; }

// it is quite common case, yet it outputs implementation details, instead of a human-readable typedef.
inline std::string toType(std::string const&)  { return "string"; }
inline std::string toType(std::wstring const&) { return "string"; }

// all CV-qualified char* types are considered just string, for the sake of simplicity
inline std::string toType(char                *) { return "string"; }
inline std::string toType(char const          *) { return "string"; }
inline std::string toType(char       volatile *) { return "string"; }
inline std::string toType(char const volatile *) { return "string"; }

// all signed numbers to be kept as just "int"
inline std::string toType(char)      { return "string"; }
inline std::string toType(short)     { return "int"; }
inline std::string toType(int)       { return "int"; }
inline std::string toType(long)      { return "int"; }
inline std::string toType(long long) { return "int"; }

// all unsigned numbers to be kept as just "unsigned int"
inline std::string toType(unsigned char)      { return "unsigned int"; }
inline std::string toType(unsigned short)     { return "unsigned int"; }
inline std::string toType(unsigned int)       { return "unsigned int"; }
inline std::string toType(unsigned long)      { return "unsigned int"; }
inline std::string toType(unsigned long long) { return "unsigned int"; }

// all fps are just "double"
inline std::string toType(float)       { return "double"; }
inline std::string toType(double)      { return "double"; }
inline std::string toType(long double) { return "double"; }

}
}
}
