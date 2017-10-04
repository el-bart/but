#pragma once
#include <string>

namespace But
{
namespace Log
{
namespace Backend
{

class Type final
{
public:
  explicit Type(std::string str): t_{ std::move(str) } { }
  //explicit Type(std::wstring str): t_{ std::move(str) } { }

  Type(Type const&) = default;
  Type& operator=(Type const&) = default;

  Type(Type&&) = default;
  Type& operator=(Type&&) = default;


  // boolean
  static auto of(bool const&) { return Type{"bool"}; }

  // it is quite common case, yet it outputs implementation details, instead of a human-readable typedef.
  static auto of(std::string const&)  { return Type{"string"}; }
  static auto of(std::wstring const&) { return Type{"string"}; }

  // all CV-qualified char* types are considered just string, for the sake of simplicity
  static auto of(char                *) { return Type{"string"}; }
  static auto of(char const          *) { return Type{"string"}; }
  static auto of(char       volatile *) { return Type{"string"}; }
  static auto of(char const volatile *) { return Type{"string"}; }

  // all signed numbers to be kept as just "int"
  static auto of(char)      { return Type{"string"}; }
  static auto of(short)     { return Type{"int"}; }
  static auto of(int)       { return Type{"int"}; }
  static auto of(long)      { return Type{"int"}; }
  static auto of(long long) { return Type{"int"}; }

  // all unsigned numbers to be kept as just "unsigned int"
  static auto of(unsigned char)      { return Type{"unsigned int"}; }
  static auto of(unsigned short)     { return Type{"unsigned int"}; }
  static auto of(unsigned int)       { return Type{"unsigned int"}; }
  static auto of(unsigned long)      { return Type{"unsigned int"}; }
  static auto of(unsigned long long) { return Type{"unsigned int"}; }

  // all fps are just "double"
  static auto of(float)       { return Type{"double"}; }
  static auto of(double)      { return Type{"double"}; }
  static auto of(long double) { return Type{"double"}; }


  std::string str() && { return std::move(t_); }
  std::string const& str() const & { return t_; }

  bool operator==(Type const& other) const { return t_ == other.t_; }
  bool operator!=(Type const& other) const { return t_ != other.t_; }
  bool operator<=(Type const& other) const { return t_ <= other.t_; }
  bool operator>=(Type const& other) const { return t_ >= other.t_; }
  bool operator< (Type const& other) const { return t_ <  other.t_; }
  bool operator> (Type const& other) const { return t_ >  other.t_; }

private:
  std::string t_;
};


inline std::ostream& operator<<(std::ostream& os, Type const& type) { return os << type.str(); }

}
}
}
