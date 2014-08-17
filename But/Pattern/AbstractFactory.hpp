#pragma once
#include <memory>
#include <string>
#include <utility>
#include <functional>
#include <unordered_map>
#include <cassert>
#include "But/Exception.hpp"
#include "But/Mpl/NamedVariadicTemplate.hpp"

namespace But
{
namespace Pattern
{

template<typename BaseType, typename ...Args>
class AbstractFactory final
{
public:
  using BasePtr      = std::unique_ptr<BaseType>;
  using BuildFunctor = std::function<BasePtr(Args...)>;
  using BuilderArgs  = Mpl::NamedVariadicTemplate<Args...>;

  BUT_DEFINE_EXCEPTION(NoSuchBuilder, Exception, "no such builder");
  BUT_DEFINE_EXCEPTION(DuplicatedEntry, Exception, "trying to insert duplicated entry");
  BUT_DEFINE_EXCEPTION(NullPointer, Exception, "builder returned null pointer");
  BUT_DEFINE_EXCEPTION(InvalidBuilder, Exception, "invalid builder");

  /** @brief named-string for builder. */
  struct Builder { std::string name_; };

  void add(Builder const& name, BuildFunctor builder)
  {
    if(not builder)
      BUT_THROW(InvalidBuilder, "registering invalid builder for '" << name.name_ << "'");
    if( builders_.find(name.name_) != end(builders_) )
      BUT_THROW(DuplicatedEntry, "builder for '" << name.name_ << "' already exists");
    builders_[name.name_] = std::move(builder);
  }

  BasePtr build(Builder const& name, Args&&... args)
  {
    const auto it = builders_.find(name.name_);
    if(it == end(builders_))
      BUT_THROW(NoSuchBuilder, name.name_);
    assert(it->second);
    auto e = it->second( std::forward<Args>(args)... );
    if(not e)
      BUT_THROW(NullPointer, "builder '" << name.name_ << "' returned nullptr");
    return e;
  }

  bool remove(Builder const& name)
  {
    return builders_.erase(name.name_) > 0u;
  }

private:
  std::unordered_map<std::string, BuildFunctor> builders_;
};

}
}
