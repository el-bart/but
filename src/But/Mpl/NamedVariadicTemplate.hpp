#pragma once

namespace But
{
namespace Mpl
{

/** @brief since C++14 does not allow to typedef variadic template arguments directory the trick must be used.
 *
 *  @par
 *  <code>
 *  template<typename ...Args>
 *  MyClass
 *  {
 *     using Params = NamedVariadicTemplate<Args...>;
 *     // ...
 *  };
 *
 *  template<typename ...Args>
 *  struct Usage;
 *
 *  template<typename ...Args>
 *  struct Usage<NamedVariadicTemplate<Args...>>
 *  {
 *    // here "Args" can be used!
 *  }
 *
 *  using Input  = MyClass<int, double>; // defines params
 *  Using Output = Usage<Input::Params>; // forwards them further
 *  </code>
 */
template<typename ...Args>
struct NamedVariadicTemplate final
{
  static constexpr auto size() { return sizeof...(Args); }
};

}
}
