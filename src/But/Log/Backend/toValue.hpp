/** @brief customization point for converting user-defined values to a printable string.
 *         ADL is used to search for proper overload. there is no default one, to avoid confusion.
 *
 *  @note while technically it could be possible to use {std,boost}::variant to keep different, possible options,
 *        quick measurements shown that it is not that beneficial. in some cases it causes speed-ups, in some
 *        it generates slow-downs. difference in performance is not clear thus does not justify complexity involved.
 */
#pragma once
#include <string>
#include "Value.hpp"

namespace But
{
namespace Log
{
namespace Backend
{

inline auto toValue(const bool v) { return Value{v}; }

inline auto toValue(std::string v) { return Value{ std::move(v) }; }
inline auto toValue(char const* v) { return Value{v}; }

inline auto toValue(const char v) { return Value{v}; }
inline auto toValue(const short v) { return Value{v}; }
inline auto toValue(const int v) { return Value{v}; }
inline auto toValue(const long v) { return Value{v}; }
inline auto toValue(const long long v) { return Value{v}; }

inline auto toValue(const unsigned char v) { return Value{v}; }
inline auto toValue(const unsigned short v) { return Value{v}; }
inline auto toValue(const unsigned int v) { return Value{v}; }
inline auto toValue(const unsigned long v) { return Value{v}; }
inline auto toValue(const unsigned long long v) { return Value{v}; }

inline auto toValue(const float v) { return Value{v}; }
inline auto toValue(const double v) { return Value{v}; }
inline auto toValue(const long double v) { return Value{v}; }

}
}
}
