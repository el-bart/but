#pragma once
#include <string>
#include <cassert>
#include "detail/parse.hpp"
#include "detail/argumentsCount.hpp"

namespace But
{
namespace Format
{

/** @brief represents a parsed format, from a given text with. set of parameters can be applied to get a final message.
 *  @note format of the parameter is defined by its type, via toString() function. it
 *        is not specified for a given type usage. this way formatting for a given parameters
 *        is always constant.
 *  @param N number of arguments
 */
template<unsigned N>
class Parsed final
{
public:
  /** @brief parses input format and constructs the object.
   *  @param format - format is a string with positional arguments, in a form:
   *          $N - expands to N-th argument.
   *          ${N} - the same as $N.
   *          ${N#some text} - the same as $N, but allowing some textual description along (useful for translations!).
   *          ${TN} - prints type of N-th argument, using typeString() function and ADL.
   *          ${TN#some text} - the same as ${TN}, but allowing some textual description along (useful for translations!).
   *          ${VN} - the same as $N.
   *          ${VN#some text} - the same as $N, but allowing some textual description along (useful for translations!).
   *          $$ - liternal '$' character.
   *  @note all numbers are 0-based (i.e. 1st arguments has index 0).
   */
  constexpr explicit Parsed(char const* format):
    ps_{ detail::parse<N+1>(format) },
    format_{format}
  { }

  //constexpr auto expectedArguments() const { return detail::argumentsCount(ps_); } // TODO: will not work with static_assert...
  static constexpr auto expectedArguments() { return N; }

  template<typename ...Args>
  std::string format(Args const& ...args) const
  {
    static_assert( sizeof...(args) == expectedArguments(), "arity missmatch between provided format and arguments to be formated" );
    assert( expectedArguments() == detail::argumentsCount(ps_) );
    // TODO: static assert here...
    return format_;
  }

private:
  detail::ParserState<N+1> ps_;
  char const* format_;
};

}
}
