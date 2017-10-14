#include <set>
#include <boost/lexical_cast.hpp>
#include "But/assert.hpp"
#include "But/Log/Destination/Common/rootElementTag.hpp"
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
auto findFreeName(json const& field, std::string const& tag, unsigned counter)
{
  auto name = tag + std::to_string(counter);
  while( field.find(name) != field.end() )
    name = tag + std::to_string( ++counter );
  return std::make_pair( std::move(name), counter );
}

template<typename V>
auto addUnique(json& field, Backend::Tag const& t, V&& v)
{
  if( field.find( t.str() ) == field.end() )
  {
    field[ t.str() ] = std::forward<V>(v);
    return false;
  }

  auto hasSpaceForFirst = false;
  auto counter = 0u;
  std::string name;
  while(true)
  {
    auto free = findFreeName(field, t.str(), counter);
    if(hasSpaceForFirst)
    {
      name = std::move(free.first);
      break;
    }
    hasSpaceForFirst = true;
    counter = free.second + 1u;
  }
  BUT_ASSERT( field.find(name) == field.end() );
  field[ std::move(name) ] = std::forward<V>(v);
  return true;
}

struct ValueVisitor final
{
  template<typename T>
  void operator()(T const& t)
  {
    BUT_ASSERT(type_);
    BUT_ASSERT(field_);
    duplicate_ = addUnique(*field_, *type_, t);
  }

  Backend::Tag const* type_{nullptr};
  json* field_{nullptr};
  bool duplicate_{false};
};


struct FieldInfoVisitor final
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
    FieldInfoVisitor fiv;
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
    auto newName = findFreeName(field_, name, 0u).first;
    BUT_ASSERT( field_.find(newName) == field_.end() );
    BUT_ASSERT( field_.find(name) != field_.end() );
    field_[ std::move(newName) ] = std::move( field_[name] );
    field_.erase(name);
  }

  json field_;
  std::set<std::string> duplicates_;
};
}


nlohmann::json toJson(Backend::FieldInfo const& fi)
{
  FieldInfoVisitor fiv;
  fi.visit(fiv);
  fiv.renameDuplicates();
  const auto& rootName = Common::rootElementTag().str();
  if( fi.tag().str() == rootName )
    return std::move( fiv.field_[rootName] );
  else
    return std::move(fiv.field_);
}

}
}
}
}
