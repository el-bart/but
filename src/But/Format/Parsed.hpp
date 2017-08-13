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
 *  @param N number of arguments
 */
template<unsigned N, unsigned MaxSegments>
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

  //constexpr auto expectedArguments() const { return detail::argumentsCount(ps_); } // TODO: will not work with static_assert...
  static constexpr auto expectedArguments() { return N; }

  template<typename ...Args>
  std::string format(Args const& ...args) const
  {
    static_assert( sizeof...(args) == expectedArguments(), "arity missmatch between provided format and arguments to be formated" );
    assert( expectedArguments() == detail::argumentsCount(ps_) );
    std::ostringstream ss;
    for(auto i=0u; i<ps_.count_; ++i)
      formatBlock(ss, ps_.segments_[i], args...);
    return ss.str();
  }

private:
  template<typename ...Args>
  std::string getArgumentType(unsigned /*pos*/, Args const& .../*args*/) const
  {
    using Log::Backend::typeString; // TODO - should be moved here...
    return "T-TODO...";
  }
  template<typename ...Args>
  std::string getArgumentValue(unsigned /*pos*/, Args const& .../*args*/) const
  {
    using Log::Backend::toString;   // TODO - should be moved here...
    return "V-TODO...";
  }


  template<typename ...Args>
  void formatBlock(std::ostringstream& ss, detail::State const& state, Args const& ...args) const
  {
    switch(state.type_)
    {
      case detail::State::Type::String:
        ss << std::string{ state.begin_, state.end_ };
        return;
      case detail::State::Type::Value:
        ss << getArgumentValue(state.referencedArgument_, args...);
        return;
      case detail::State::Type::TypeName:
        ss << getArgumentType(state.referencedArgument_, args...);
        return;
    }
    assert(!"missing type handle");
  }

  const detail::ParserState<MaxSegments> ps_;
  char const* format_;
};

}
}
