#pragma once
#include <string>

namespace But
{
namespace Format
{

inline auto toString(std::string str) { return str; }
inline auto toString(char c) { return std::string(1,c); }
inline auto toString(char* c) { return std::string{c}; }
inline auto toString(char const* c) { return std::string{c}; }
//inline auto toString(std::wstring str) { return str; }

inline auto toString(const bool b) { return b ? "true" : "false"; }

//inline auto toString(const int8_t i) { return std::to_string(i); }
inline auto toString(const uint8_t ui) { return std::to_string(ui); }

inline auto toString(const int16_t i) { return std::to_string(i); }
inline auto toString(const uint16_t ui) { return std::to_string(ui); }

inline auto toString(const int32_t i) { return std::to_string(i); }
inline auto toString(const uint32_t ui) { return std::to_string(ui); }

inline auto toString(const int64_t i) { return std::to_string(i); }
inline auto toString(const uint64_t ui) { return std::to_string(ui); }

inline auto toString(const long long i) { return std::to_string(i); }
inline auto toString(const unsigned long long ui) { return std::to_string(ui); }

inline auto toString(const float fp) { return std::to_string(fp); }
inline auto toString(const long double fp) { return std::to_string(fp); }
inline auto toString(const double fp) { return std::to_string(fp); }

}
}
