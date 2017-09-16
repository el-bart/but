/** @brief customization point for converting user-defined values to a printable string.
 *         ADL is used to search for proper overload. there is no default one, to avoid confusion.
 *
 *  @note while technically it could be possible to use {std,boost}::variant to keep different, possible options,
 *        quick measurements shown that it is not that beneficial. in some cases it causes speed-ups, in some
 *        it generates slow-downs. difference in performance is not clear thus does not justify complexity involved.
 */
#pragma once
#include <string>

namespace But
{
namespace Log
{
namespace Backend
{

inline std::string toValue(const bool v) { return v ? "true" : "false"; }

inline std::string toValue(std::string v) { return v; }
inline std::string toValue(char const* v) { return v; }

inline std::string toValue(const char v) { return std::string(1, v); }
inline std::string toValue(const short v) { return std::to_string( int{v} ); }
inline std::string toValue(const int v) { return std::to_string(v); }
inline std::string toValue(const long v) { return std::to_string(v); }
inline std::string toValue(const long long v) { return std::to_string(v); }

inline std::string toValue(const unsigned char v) { return std::to_string( unsigned{v} ); }
inline std::string toValue(const unsigned short v) { return std::to_string( unsigned{v} ); }
inline std::string toValue(const unsigned int v) { return std::to_string(v); }
inline std::string toValue(const unsigned long v) { return std::to_string(v); }
inline std::string toValue(const unsigned long long v) { return std::to_string(v); }

inline std::string toValue(const float v) { return std::to_string(v); }
inline std::string toValue(const double v) { return std::to_string(v); }
inline std::string toValue(const long double v) { return std::to_string(v); }

}
}
}
