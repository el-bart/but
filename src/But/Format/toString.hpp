#pragma once
#include <string>

namespace But
{
namespace Format
{

auto toString(std::string str) { return str; }
auto toString(char c) { return std::string(1,c); }
auto toString(char* c) { return std::string{c}; }
auto toString(char const* c) { return std::string{c}; }
//auto toString(std::wstring str) { return str; }

auto toString(const bool b) { return b ? "true" : "false"; }

//auto toString(const int8_t i) { return std::to_string(i); }
auto toString(const uint8_t ui) { return std::to_string(ui); }

auto toString(const int16_t i) { return std::to_string(i); }
auto toString(const uint16_t ui) { return std::to_string(ui); }

auto toString(const int32_t i) { return std::to_string(i); }
auto toString(const uint32_t ui) { return std::to_string(ui); }

auto toString(const int64_t i) { return std::to_string(i); }
auto toString(const uint64_t ui) { return std::to_string(ui); }

auto toString(const long long i) { return std::to_string(i); }
auto toString(const unsigned long long ui) { return std::to_string(ui); }

auto toString(const float fp) { return std::to_string(fp); }
auto toString(const long double fp) { return std::to_string(fp); }
auto toString(const double fp) { return std::to_string(fp); }

}
}
