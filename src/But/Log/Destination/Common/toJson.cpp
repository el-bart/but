#include <cassert>
#include <boost/lexical_cast.hpp>
#include "toJson.hpp"

using json = nlohmann::json;

namespace But
{
namespace Log
{
namespace Destination
{
namespace Common
{

namespace
{
template<typename V>
void addUnique(json& field, Backend::Tag const& t, V&& v)
{
  auto counter = 0u;
  auto name = t.str();
  for( auto it = field.find(name);
       it != field.end();
       name = t.str() + std::to_string(counter), it = field.find(name) )
    ++counter;
  assert( field.find(name) == field.end() );
  field[ std::move(name) ] = std::forward<V>(v);
}

struct ValueVisitor final
{
  template<typename T>
  void operator()(T const& t)
  {
    assert(type_);
    assert(field_);
    addUnique(*field_, *type_, t);
  }

  Backend::Tag const* type_{nullptr};
  json* field_{nullptr};
};


struct NestedFieldInfoVisitor final
{
  void operator()(Backend::Tag const& t, Backend::Value const& v)
  {
    ValueVisitor vv{&t, &field_};
    v.visit(vv);
  }
  void operator()(Backend::Tag const& t, std::vector<Backend::FieldInfo>const& fis)
  {
    NestedFieldInfoVisitor fiv;
    for(auto& e: fis)
      e.visit(fiv);
    addUnique( field_, t, std::move(fiv.field_) );
  }

  json field_;
};

struct FieldInfoVisitor final
{
  void operator()(Backend::Tag const& t, Backend::Value const& v)
  {
    json tmp;
    ValueVisitor vv{&t, &tmp};
    v.visit(vv);
    field_.push_back( std::move(tmp) );
  }
  void operator()(Backend::Tag const& /*t*/, std::vector<Backend::FieldInfo>const& fis)
  {
    for(auto& e: fis)
    {
      NestedFieldInfoVisitor fiv;
      e.visit(fiv);
      field_.push_back( std::move(fiv.field_) );
    }
  }

  json field_ = json::array();
};
}


nlohmann::json toJson(Backend::FieldInfo const& fi)
{
  FieldInfoVisitor fiv;
  fi.visit(fiv);
  return std::move(fiv.field_);
}

}
}
}
}
