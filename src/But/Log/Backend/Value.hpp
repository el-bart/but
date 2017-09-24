#pragma once
#include <string>
#include <cinttypes>
#include <boost/variant.hpp>
#include "detail/StreamVisitor.hpp"

namespace But
{
namespace Log
{
namespace Backend
{

class Value final
{
public:
  explicit Value(char c): Value{ std::string(1,c) } { }
  explicit Value(char* c): Value{ std::string{c} } { }
  explicit Value(char const* c): Value{ std::string{c} } { }
  explicit Value(std::string str): v_{ std::move(str) } { }
  //explicit Value(std::wstring str): v_{ std::move(str) } { }

  explicit Value(const bool b): v_{b} { }

  //Value(const int8_t i): Value{ static_cast<int64_t>(i) } { } // this is 'char' and thus 'string'...
  explicit Value(const uint8_t ui): Value{ static_cast<uint64_t>(ui) } { }

  explicit Value(const int16_t i): Value{ static_cast<int64_t>(i) } { }
  explicit Value(const uint16_t ui): Value{ static_cast<uint64_t>(ui) } { }

  explicit Value(const int32_t i): Value{ static_cast<int64_t>(i) } { }
  explicit Value(const uint32_t ui): Value{ static_cast<uint64_t>(ui) } { }

  explicit Value(const int64_t i): v_{i} { }
  explicit Value(const uint64_t ui): v_{ui} { }

  explicit Value(const long long i): Value{ static_cast<int64_t>(i) } { }
  explicit Value(const unsigned long long ui): Value{ static_cast<uint64_t>(ui) } { }

  explicit Value(const float fp): Value{ static_cast<double>(fp) } { }
  explicit Value(const long double fp): Value{ static_cast<double>(fp) } { }
  explicit Value(const double fp): v_{fp} { }

  Value(Value const&) = default;
  Value& operator=(Value const&) = default;

  Value(Value&&) = default;
  Value& operator=(Value&&) = default;

  // TODO: get should throw local exception type - not implementation-detail...
  template<typename T>
  T get() && { return std::move( boost::get<T>( std::move(v_) ) ); }
  template<typename T>
  T const& get() const & { return boost::get<T>(v_); }

  /* TODO: enable when std::variant<> is used...
  template<typename V>
  void visit(V&& v) && { boost::apply_visitor( std::forward<V>(v), v_ ); }
  */
  template<typename V>
  void visit(V&& v) { boost::apply_visitor( std::forward<V>(v), v_ ); }
  template<typename V>
  void visit(V&& v) const { boost::apply_visitor( std::forward<V>(v), v_ ); }

  bool operator==(Value const& other) const { return v_ == other.v_; }
  bool operator!=(Value const& other) const { return v_ != other.v_; }
  bool operator<=(Value const& other) const { return v_ <= other.v_; }
  bool operator>=(Value const& other) const { return v_ >= other.v_; }
  bool operator< (Value const& other) const { return v_ <  other.v_; }
  bool operator> (Value const& other) const { return v_ >  other.v_; }

private:
  // TODO: std::variant<>, once C++17 is out...
  using Variant = boost::variant<std::string, uint64_t, int64_t, double, bool>;
  Variant v_;
};


inline std::ostream& operator<<(std::ostream& os, Value const& value)
{
  value.visit( detail::StreamVisitor{&os} );
  return os;
}

}
}
}
