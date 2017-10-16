#pragma once
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
inline auto toFieldInfo(char               * v) { return FieldInfo{ Tag{"string"}, Value{v} }; }
inline auto toFieldInfo(char const         * v) { return FieldInfo{ Tag{"string"}, Value{v} }; }
inline auto toFieldInfo(char       volatile* v) { return FieldInfo{ Tag{"string"}, Value{v} }; }
inline auto toFieldInfo(char const volatile* v) { return FieldInfo{ Tag{"string"}, Value{v} }; }

// all signed numbers to be kept as just "int"
inline auto toFieldInfo(char v)      { return FieldInfo{ Tag{"string"}, Value{v} }; }
inline auto toFieldInfo(short v)     { return FieldInfo{ Tag{"int"}, Value{v} }; }
inline auto toFieldInfo(int v)       { return FieldInfo{ Tag{"int"}, Value{v} }; }
inline auto toFieldInfo(long v)      { return FieldInfo{ Tag{"int"}, Value{v} }; }
inline auto toFieldInfo(long long v) { return FieldInfo{ Tag{"int"}, Value{v} }; }

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

}
}
}
