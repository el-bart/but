#pragma once
#include <memory>
#include <string>
#include <utility>
#include <functional>
#include <unordered_map>
#include "But/assert.hpp"
#include "But/Exception.hpp"
#include "But/Mpl/NamedVariadicTemplate.hpp"

namespace But
{
namespace Pattern
{

/** @brief compact in syntax implementation of abstract factory pattern.
 *
 *  @par
 *  class is a template of a user's base class type and list of arguments each builder takes.
 *  since it is using functors internally, lambdas can be used for creating builders as well.
 *  this gives a very compact syntax for addition and operation.
 *
 *  @par
 *  for example assume having class 'Base' and one implementation called 'Derived'. builders take
 *  'double' and 'int' as parameters, to pass to a proper c-tor. usage can look like this:
 *  <code>
 *    struct Base;
 *    struct Derived
 *    {
 *      Derived(double, int);
 *    };
 *
 *    using MyFactory = AbstractFactory<Base, double, int>;
 *    MyFactory f;
 *    f.add( MyFactory::Builder{"derived"}, [](double d, int i) { return std::make_unique<Derived>(d,i); } );
 *    auto e = f.build(MyFactory::Builder{"derived"}); // 'e' is of a type 'Derived'
 *  </code>
 *
 *  @note
 *  factory methods must return std::unique_ptr<BaseType>.
 */
template<typename BaseType, typename ...Args>
class AbstractFactory final
{
public:
  using BasePtr      = std::unique_ptr<BaseType>;
  using BuildFunctor = std::function<BasePtr(Args...)>;
  using BuilderArgs  = Mpl::NamedVariadicTemplate<Args...>;

  BUT_DEFINE_EXCEPTION(NoSuchBuilder, Exception, "no such builder");
  BUT_DEFINE_EXCEPTION(DuplicatedEntry, Exception, "trying to insert duplicated entry");
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

  /** @brief builds given element type, with given arguments.
   *  @returns non-nullptr pointer to the constructed element.
   */
  BasePtr build(Builder const& name, Args&&... args)
  {
    const auto it = builders_.find(name.name_);
    if(it == end(builders_))
      BUT_THROW(NoSuchBuilder, name.name_);
    assert(it->second);
    auto e = it->second( std::forward<Args>(args)... );
    assert( e.get() && "builders must return non-null pointers" );
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
