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
