#pragma once
#include <vector>
#include <boost/variant.hpp>
#include "Tag.hpp"
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
    tag_{ Tag::of(value) },
    variant_{ Value{ std::forward<T>(value) } }
  { }

  FieldInfo(Value const& value) = delete;
  FieldInfo(Value&& value) = delete;

  FieldInfo(Tag tag, Value value):
    tag_{ std::move(tag) },
    variant_{ std::move(value) }
  { }
  FieldInfo(Tag tag, std::vector<FieldInfo> fi):
    tag_{ std::move(tag) },
    variant_{ std::move(fi) }
  { }

  FieldInfo(FieldInfo&&) = default;
  FieldInfo& operator=(FieldInfo&&) = default;

  FieldInfo(FieldInfo const&) = default;
  FieldInfo& operator=(FieldInfo const&) = default;

  bool operator==(FieldInfo const& other) const { return tag_ == other.tag_ && variant_ == other.variant_; }
  bool operator!=(FieldInfo const& other) const { return not ( *this == other ); }

  auto const& tag() const & { return tag_; }
  auto const& value() const & { return boost::get<Value>(variant_); }
  auto const& array() const & { return boost::get<std::vector<FieldInfo>>(variant_); }

  auto&& tag() && { return std::move(tag_); }
  auto&& value() && { return boost::get<Value>( std::move(variant_) ); }
  auto&& array() && { return boost::get<std::vector<FieldInfo>>( std::move(variant_) ); }

  /** @brief visitor takes tag and value parametrs, like this:
   *  <code>
   *    struct Visitor
   *    {
   *      void operator()(Tag const& t, Value const& v)
   *      {
   *        cout << '/' << t << '=' << v;
   *      }
   *      void operator()(Tag const& t, std::vector<FieldInfo> const& fis)
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
  void visit(F&& f) { boost::apply_visitor( [&](auto& e) { f(tag_, e); }, variant_ ); }
  template<typename F>
  void visit(F&& f) const { boost::apply_visitor( [&](auto const& e) { f(tag_, e); }, variant_ ); }

  auto retag(Tag t) &&
  {
    tag_ = std::move(t);
    return std::move(*this);
  }

private:
  // TODO: std::variant<> when C++17 is here
  using Variant = boost::variant<Value, std::vector<FieldInfo>>;

  Tag tag_;
  Variant variant_;
};

}
}
}
