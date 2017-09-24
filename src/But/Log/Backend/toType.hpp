/** @brief customization point for converting user-defined values to a printable string.
 *         ADL is used to search for proper overload.
 */
#pragma once
#include "Type.hpp"
#include <boost/type_index.hpp>

namespace But
{
namespace Log
{
namespace Backend
{

// boolean
inline auto toType(bool const&) { return Type{"bool"}; }

// it is quite common case, yet it outputs implementation details, instead of a human-readable typedef.
inline auto toType(std::string const&)  { return Type{"string"}; }
inline auto toType(std::wstring const&) { return Type{"string"}; }

// all CV-qualified char* types are considered just string, for the sake of simplicity
inline auto toType(char                *) { return Type{"string"}; }
inline auto toType(char const          *) { return Type{"string"}; }
inline auto toType(char       volatile *) { return Type{"string"}; }
inline auto toType(char const volatile *) { return Type{"string"}; }

// all signed numbers to be kept as just "int"
inline auto toType(char)      { return Type{"string"}; }
inline auto toType(short)     { return Type{"int"}; }
inline auto toType(int)       { return Type{"int"}; }
inline auto toType(long)      { return Type{"int"}; }
inline auto toType(long long) { return Type{"int"}; }

// all unsigned numbers to be kept as just "unsigned int"
inline auto toType(unsigned char)      { return Type{"unsigned int"}; }
inline auto toType(unsigned short)     { return Type{"unsigned int"}; }
inline auto toType(unsigned int)       { return Type{"unsigned int"}; }
inline auto toType(unsigned long)      { return Type{"unsigned int"}; }
inline auto toType(unsigned long long) { return Type{"unsigned int"}; }

// all fps are just "double"
inline auto toType(float)       { return Type{"double"}; }
inline auto toType(double)      { return Type{"double"}; }
inline auto toType(long double) { return Type{"double"}; }

}
}
}
