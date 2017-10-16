/** @brief customization point for converting user-defined values to a printable string.
 *         ADL is used to search for proper overload.
 */
#pragma once
#include <But/Log/Backend/Tag.hpp>

namespace But
{
namespace Log
{
namespace Backend
{

// boolean
inline auto toType(bool const&) { return Tag{"bool"}; }

// it is quite common case, yet it outputs implementation details, instead of a human-readable typedef.
inline auto toType(std::string const&)  { return Tag{"string"}; }
inline auto toType(std::wstring const&) { return Tag{"string"}; }

// all CV-qualified char* types are considered just string, for the sake of simplicity
inline auto toType(char                *) { return Tag{"string"}; }
inline auto toType(char const          *) { return Tag{"string"}; }
inline auto toType(char       volatile *) { return Tag{"string"}; }
inline auto toType(char const volatile *) { return Tag{"string"}; }

// all signed numbers to be kept as just "int"
inline auto toType(char)      { return Tag{"string"}; }
inline auto toType(short)     { return Tag{"int"}; }
inline auto toType(int)       { return Tag{"int"}; }
inline auto toType(long)      { return Tag{"int"}; }
inline auto toType(long long) { return Tag{"int"}; }

// all unsigned numbers to be kept as just "unsigned int"
inline auto toType(unsigned char)      { return Tag{"unsigned int"}; }
inline auto toType(unsigned short)     { return Tag{"unsigned int"}; }
inline auto toType(unsigned int)       { return Tag{"unsigned int"}; }
inline auto toType(unsigned long)      { return Tag{"unsigned int"}; }
inline auto toType(unsigned long long) { return Tag{"unsigned int"}; }

// all fps are just "double"
inline auto toType(float)       { return Tag{"double"}; }
inline auto toType(double)      { return Tag{"double"}; }
inline auto toType(long double) { return Tag{"double"}; }

}
}
}
