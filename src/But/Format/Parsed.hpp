#pragma once
#include <string>
#include <sstream>
#include <cassert>
#include "detail/parse.hpp"
#include "detail/argumentsCount.hpp"
#include "But/Log/Backend/toString.hpp"
#include "But/Log/Backend/typeString.hpp"

namespace But
{
namespace Format
{

/** @brief represents a parsed format, from a given text with. set of parameters can be applied to get a final message.
 *  @note format of the parameter is defined by its type, via toString() function. it
 *        is not specified for a given type usage. this way formatting for a given parameters
 *        is always constant.
 */
template<unsigned ArgumentsCount, unsigned MaxSegments>
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
   *  @note all numbers are 0-based (i.e. 1st argument has index 0).
   */
  constexpr explicit Parsed(char const* format):
    ps_{ detail::parse<MaxSegments>(format) },
    format_{format}
  { }

  constexpr auto inputFormat() const { return format_; }
  //constexpr auto expectedArguments() const { return detail::argumentsCount(ps_); } // note: would not work with static_assert...
  static constexpr auto expectedArguments() { return ArgumentsCount; }
  static constexpr auto maxSegments() { return MaxSegments; }

  template<typename ...Args>
  std::string format(Args const& ...args) const
  {
    static_assert( sizeof...(args) == expectedArguments(), "arity missmatch between provided format and arguments to be formated" );
    assert( expectedArguments() == detail::argumentsCount(ps_) );
    std::ostringstream os;
    for(auto i=0u; i<ps_.count_; ++i)
      formatBlock(os, ps_.segments_[i], args...);
    return os.str();
  }

private:
  template<typename F>
  std::string processArgument(F&& /*f*/, const unsigned /*pos*/) const
  {
    assert(!"this overload is never really called");
    std::terminate();
  }
  template<typename F, typename Head>
  std::string processArgument(F&& f, const unsigned pos, Head const& head) const
  {
    (void)pos;
    assert( pos == 0u && "format is not alligned with arguments" );
    return f(head);
  }
  template<typename F, typename Head, typename ...Tail>
  std::string processArgument(F&& f, const unsigned pos, Head const& head, Tail const& ...tail) const
  {
    if( pos == 0u )
      return f(head);
    return processArgument( std::forward<F>(f), pos-1u, tail... );
  }

  template<typename ...Args>
  std::string getArgumentType(const unsigned pos, Args const& ...args) const
  {
    using Log::Backend::typeString;
    return processArgument( [](auto& e) { return typeString(e); },  pos, args... );
  }
  template<typename ...Args>
  std::string getArgumentValue(const unsigned pos, Args const& ...args) const
  {
    using Log::Backend::toString;
    return processArgument( [](auto& e) { return toString(e); },  pos, args... );
  }


  template<typename ...Args>
  void formatBlock(std::ostringstream& os, detail::State const& state, Args const& ...args) const
  {
    switch(state.type_)
    {
      case detail::State::Type::String:
        os.write( state.begin_, state.end_ - state.begin_ );
        return;
      case detail::State::Type::Value:
        os << getArgumentValue(state.referencedArgument_, args...);
        return;
      case detail::State::Type::TypeName:
        os << getArgumentType(state.referencedArgument_, args...);
        return;
    }
    assert(!"missing type handle");
  }

  const detail::ParserState<MaxSegments> ps_;
  char const* format_;
};

}
}
