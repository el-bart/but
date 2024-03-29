#pragma once
#include <string>
#include <string_view>

namespace But
{
namespace Format
{

inline auto toString(std::string str) { return str; }
inline auto toString(std::string_view str) { return std::string{str}; }
inline auto toString(char c) { return std::string(1,c); }
inline auto toString(char* c) { return std::string{c}; }
inline auto toString(char const* c) { return std::string{c}; }
//inline auto toString(std::wstring str) { return str; }

inline auto toString(const bool b) { return b ? "true" : "false"; }

inline auto toString(const signed char i) { return std::to_string(i); }
inline auto toString(const unsigned char ui) { return std::to_string(ui); }

inline auto toString(const signed short i) { return std::to_string(i); }
inline auto toString(const unsigned short ui) { return std::to_string(ui); }

inline auto toString(const signed int i) { return std::to_string(i); }
inline auto toString(const unsigned int ui) { return std::to_string(ui); }

inline auto toString(const long i) { return std::to_string(i); }
inline auto toString(const unsigned long ui) { return std::to_string(ui); }

inline auto toString(const long long i) { return std::to_string(i); }
inline auto toString(const unsigned long long ui) { return std::to_string(ui); }

inline auto toString(const float fp) { return std::to_string(fp); }
inline auto toString(const long double fp) { return std::to_string(fp); }
inline auto toString(const double fp) { return std::to_string(fp); }

}
}
