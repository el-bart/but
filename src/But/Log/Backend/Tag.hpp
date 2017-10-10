#pragma once
#include <string>

namespace But
{
namespace Log
{
namespace Backend
{

class Tag final
{
public:
  explicit Tag(std::string str): t_{ std::move(str) } { }
  //explicit Tag(std::wstring str): t_{ std::move(str) } { }

  Tag(Tag const&) = default;
  Tag& operator=(Tag const&) = default;

  Tag(Tag&&) = default;
  Tag& operator=(Tag&&) = default;


  // boolean
  static auto of(bool const&) { return Tag{"bool"}; }

  // it is quite common case, yet it outputs implementation details, instead of a human-readable typedef.
  static auto of(std::string const&)  { return Tag{"string"}; }
  static auto of(std::wstring const&) { return Tag{"string"}; }

  // all CV-qualified char* types are considered just string, for the sake of simplicity
  static auto of(char                *) { return Tag{"string"}; }
  static auto of(char const          *) { return Tag{"string"}; }
  static auto of(char       volatile *) { return Tag{"string"}; }
  static auto of(char const volatile *) { return Tag{"string"}; }

  // all signed numbers to be kept as just "int"
  static auto of(char)      { return Tag{"string"}; }
  static auto of(short)     { return Tag{"int"}; }
  static auto of(int)       { return Tag{"int"}; }
  static auto of(long)      { return Tag{"int"}; }
  static auto of(long long) { return Tag{"int"}; }

  // all unsigned numbers to be kept as just "unsigned int"
  static auto of(unsigned char)      { return Tag{"unsigned int"}; }
  static auto of(unsigned short)     { return Tag{"unsigned int"}; }
  static auto of(unsigned int)       { return Tag{"unsigned int"}; }
  static auto of(unsigned long)      { return Tag{"unsigned int"}; }
  static auto of(unsigned long long) { return Tag{"unsigned int"}; }

  // all fps are just "double"
  static auto of(float)       { return Tag{"double"}; }
  static auto of(double)      { return Tag{"double"}; }
  static auto of(long double) { return Tag{"double"}; }


  std::string str() && { return std::move(t_); }
  std::string const& str() const & { return t_; }

  bool operator==(Tag const& other) const { return t_ == other.t_; }
  bool operator!=(Tag const& other) const { return t_ != other.t_; }
  bool operator<=(Tag const& other) const { return t_ <= other.t_; }
  bool operator>=(Tag const& other) const { return t_ >= other.t_; }
  bool operator< (Tag const& other) const { return t_ <  other.t_; }
  bool operator> (Tag const& other) const { return t_ >  other.t_; }

private:
  std::string t_;
};


inline std::ostream& operator<<(std::ostream& os, Tag const& type) { return os << type.str(); }

}
}
}
