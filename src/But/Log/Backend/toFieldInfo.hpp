#pragma once
#include <tuple>
#include <chrono>
#include <vector>
#include <utility>
#include <But/Log/Backend/FieldInfo.hpp>

namespace But
{
namespace Log
{
namespace Backend
{

// identity
inline auto  toFieldInfo(FieldInfo&& fi) { return std::move(fi); }
inline auto& toFieldInfo(FieldInfo const& fi) { return fi; }

// boolean
inline auto toFieldInfo(bool v) { return FieldInfo{ Tag{"bool"}, Value{v} }; }

// it is quite common case, yet it outputs implementation details, instead of a human-readable typedef.
inline auto toFieldInfo(std::string v)  { return FieldInfo{ Tag{"string"}, Value{ std::move(v) } }; }
//inline auto toFieldInfo(std::wstring v) { return FieldInfo{ Tag{"string"}, Value{ std::move(v) } }; }

// all CV-qualified char* types are considered just string, for the sake of simplicity
inline auto toFieldInfo(char       * v) { return FieldInfo{ Tag{"string"}, Value{v} }; }
inline auto toFieldInfo(char const * v) { return FieldInfo{ Tag{"string"}, Value{v} }; }
// char is a string as well
inline auto toFieldInfo(char v)                 { return FieldInfo{ Tag{"string"}, Value{v} }; }

// all signed numbers to be kept as just "int"
inline auto toFieldInfo(signed char v)        { return FieldInfo{ Tag{"int"}, Value{v} }; }
inline auto toFieldInfo(signed short v)       { return FieldInfo{ Tag{"int"}, Value{v} }; }
inline auto toFieldInfo(signed int v)         { return FieldInfo{ Tag{"int"}, Value{v} }; }
inline auto toFieldInfo(signed long v)        { return FieldInfo{ Tag{"int"}, Value{v} }; }
inline auto toFieldInfo(signed long long v)   { return FieldInfo{ Tag{"int"}, Value{v} }; }

// all unsigned numbers to be kept as just "unsigned int"
inline auto toFieldInfo(unsigned char v)      { return FieldInfo{ Tag{"unsigned int"}, Value{v} }; }
inline auto toFieldInfo(unsigned short v)     { return FieldInfo{ Tag{"unsigned int"}, Value{v} }; }
inline auto toFieldInfo(unsigned int v)       { return FieldInfo{ Tag{"unsigned int"}, Value{v} }; }
inline auto toFieldInfo(unsigned long v)      { return FieldInfo{ Tag{"unsigned int"}, Value{v} }; }
inline auto toFieldInfo(unsigned long long v) { return FieldInfo{ Tag{"unsigned int"}, Value{v} }; }

// all fps are just "double"
inline auto toFieldInfo(float v)       { return FieldInfo{ Tag{"double"}, Value{v} }; }
inline auto toFieldInfo(double v)      { return FieldInfo{ Tag{"double"}, Value{v} }; }
inline auto toFieldInfo(long double v) { return FieldInfo{ Tag{"double"}, Value{v} }; }

// time units
inline auto toFieldInfo(std::chrono::nanoseconds v) { return FieldInfo{ Tag{"time [ns]"}, Value{v.count()} }; }
inline auto toFieldInfo(std::chrono::microseconds v) { return FieldInfo{ Tag{"time [us]"}, Value{v.count()} }; }
inline auto toFieldInfo(std::chrono::milliseconds v) { return FieldInfo{ Tag{"time [ms]"}, Value{v.count()} }; }
inline auto toFieldInfo(std::chrono::seconds v) { return FieldInfo{ Tag{"time [s]"}, Value{v.count()} }; }
inline auto toFieldInfo(std::chrono::minutes v) { return FieldInfo{ Tag{"time [min]"}, Value{v.count()} }; }
inline auto toFieldInfo(std::chrono::hours v) { return FieldInfo{ Tag{"time [h]"}, Value{v.count()} }; }
/* TODO: waiting for C++20...
inline auto toFieldInfo(std::chrono::days v) { return FieldInfo{ Tag{"time [d]"}, Value{v.count()} }; }
inline auto toFieldInfo(std::chrono::mounts v) { return FieldInfo{ Tag{"time [mon]"}, Value{v.count()} }; }
inline auto toFieldInfo(std::chrono::years v) { return FieldInfo{ Tag{"time [y]"}, Value{v.count()} }; }
*/

FieldInfo toFieldInfo(std::exception const& ex);

template<typename T>
inline auto toFieldInfo(std::vector<T> v);

template<typename F, typename S>
inline auto toFieldInfo(std::pair<F,S> v);

template<typename ...Args>
inline auto toFieldInfo(std::tuple<Args...> t);

}
}
}

#include <But/Log/Backend/detail/toFieldInfo.hpp>
