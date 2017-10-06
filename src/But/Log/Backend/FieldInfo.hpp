#pragma once
#include <vector>
#include <boost/variant.hpp>
#include "Type.hpp"
#include "Value.hpp"

namespace But
{
namespace Log
{
namespace Backend
{

class FieldInfo final
{
public:
  template<typename T>
  explicit FieldInfo(T&& value):
    type_{ Type::of(value) },
    variant_{ Value{ std::forward<T>(value) } }
  { }

  FieldInfo(Value const& value) = delete;
  FieldInfo(Value&& value) = delete;

  FieldInfo(Type type, Value value):
    type_{ std::move(type) },
    variant_{ std::move(value) }
  { }
  FieldInfo(Type type, std::vector<FieldInfo> fi):
    type_{ std::move(type) },
    variant_{ std::move(fi) }
  { }

  FieldInfo(FieldInfo&&) = default;
  FieldInfo& operator=(FieldInfo&&) = default;

  FieldInfo(FieldInfo const&) = default;
  FieldInfo& operator=(FieldInfo const&) = default;

  bool operator==(FieldInfo const& other) const { return type_ == other.type_ && variant_ == other.variant_; }
  bool operator!=(FieldInfo const& other) const { return not ( *this == other ); }

  auto const& type() const & { return type_; }
  auto const& value() const & { return boost::get<Value>(variant_); }
  auto const& array() const & { return boost::get<std::vector<FieldInfo>>(variant_); }

  auto&& type() && { return std::move(type_); }
  auto&& value() && { return boost::get<Value>( std::move(variant_) ); }
  auto&& array() && { return boost::get<std::vector<FieldInfo>>( std::move(variant_) ); }

  /** @brief visitor takes type and value parametrs, like this:
   *  <code>
   *    struct Visitor
   *    {
   *      void operator()(Type const& t, Value const& v)
   *      {
   *        cout << '/' << t << '=' << v;
   *      }
   *      void operator()(Type const& t, std::vector<FieldInfo> const& fis)
   *      {
   *        cout << '/' << t << "=[";
   *        for(auto& e: fis)
   *          e.visit(*this);
   *        ss_ << "]";
   *      }
   *    };
   *  </code>
   */
  template<typename F>
  void visit(F&& f) { boost::apply_visitor( [&](auto& e) { f(type_, e); }, variant_ ); }
  template<typename F>
  void visit(F&& f) const { boost::apply_visitor( [&](auto const& e) { f(type_, e); }, variant_ ); }

private:
  Type type_;
  // TODO: std::variant<> when C++17 is here
  boost::variant<Value, std::vector<FieldInfo>> variant_;
};

}
}
}
