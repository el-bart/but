#include <set>
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
auto addUnique(json& field, Backend::Tag const& t, V&& v)
{
  auto duplicates = false;
  auto counter = 0u;
  auto name = t.str();
  for( auto it = field.find(name); it != field.end(); it = field.find(name) )
  {
    duplicates = true;
    ++counter;
    name = t.str() + std::to_string(counter);
  }
  assert( field.find(name) == field.end() );
  field[ std::move(name) ] = std::forward<V>(v);
  return duplicates;
}

struct ValueVisitor final
{
  template<typename T>
  void operator()(T const& t)
  {
    assert(type_);
    assert(field_);
    duplicate_ = addUnique(*field_, *type_, t);
  }

  Backend::Tag const* type_{nullptr};
  json* field_{nullptr};
  bool duplicate_{false};
};


struct NestedFieldInfoVisitor final
{
  void operator()(Backend::Tag const& t, Backend::Value const& v)
  {
    ValueVisitor vv{&t, &field_};
    v.visit(vv);
    if(vv.duplicate_)
      duplicates_.insert( t.str() );
  }

  void operator()(Backend::Tag const& t, std::vector<Backend::FieldInfo>const& fis)
  {
    NestedFieldInfoVisitor fiv;
    for(auto& e: fis)
      e.visit(fiv);
    fiv.renameDuplicates();

    if( addUnique( field_, t, std::move(fiv.field_) ) )
      duplicates_.insert( t.str() );
  }

  void renameDuplicates()
  {
    for(auto& e: duplicates_)
      renameDuplicate(e);
  }
  void renameDuplicate(std::string const& name)
  {
    assert( field_.find(name) != field_.end() );
    auto newName = name + "0";
    //if( field_.find(newName) != field.name
    field_[ std::move(newName) ] = std::move( field_[name] );
    field_.erase(name);
  }

  json field_;
  std::set<std::string> duplicates_;
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
      fiv.renameDuplicates();
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
