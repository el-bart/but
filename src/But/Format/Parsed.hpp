#pragma once
#include <string>
#include <sstream>
#include "But/assert.hpp"
#include "detail/parse.hpp"
#include "detail/argumentsCount.hpp"
#include "detail/allArgumentsUsed.hpp"
#include "But/Log/Backend/toValue.hpp"
#include "But/Log/Backend/toType.hpp"
#include "Invalid.hpp"
#include "detail/StreamVisitor.hpp"

namespace But
{
namespace Format
{

/** @brief represents a parsed format, from a given text with. set of parameters can be applied to get a final message.
 *  @note format of the parameter is defined by its type, via toValue() function. it
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
   *          ${TN} - prints type of N-th argument, using toType() function and ADL.
   *          ${TN#some text} - the same as ${TN}, but allowing some textual description along (useful for translations!).
   *          ${VN} - the same as $N.
   *          ${VN#some text} - the same as $N, but allowing some textual description along (useful for translations!).
   *          $$ - liternal '$' character.
   *  @note all numbers are 0-based (i.e. 1st argument has index 0).
   */
  constexpr explicit Parsed(char const* format):
    ps_{ detail::parseCt<MaxSegments>(format) },
    format_{format}
  { }

  constexpr auto inputFormat() const { return format_; }
  //constexpr auto expectedArguments() const { return detail::argumentsCount(ps_); } // note: would not work with static_assert...
  static constexpr auto expectedArguments() { return ArgumentsCount; }

  template<typename ...Args>
  std::string format(Args const& ...args) const
  {
    static_assert( sizeof...(args) == expectedArguments(), "arity missmatch between provided format and arguments to be formated" );
    BUT_ASSERT( expectedArguments() == detail::argumentsCount(ps_) );
    std::ostringstream os;
    for(auto& e: ps_.segments_)
      formatBlock(os, e, args...);
    return os.str();
  }

private:
  template<typename F>
  void processArgument(F&& /*f*/, const unsigned /*pos*/) const
  {
    BUT_ASSERT(!"this overload should never really be called");
    std::terminate();
  }
  template<typename F, typename Head>
  void processArgument(F&& f, const unsigned pos, Head const& head) const
  {
    (void)pos;
    BUT_ASSERT( pos == 0u && "format is not alligned with arguments" );
    f(head);
  }
  template<typename F, typename Head, typename ...Tail>
  void processArgument(F&& f, const unsigned pos, Head const& head, Tail const& ...tail) const
  {
    // TODO: replace this with a compile-time-generated construct...
    if( pos == 0u )
      f(head);
    else
      processArgument( std::forward<F>(f), pos-1u, tail... );
  }

  template<typename ...Args>
  void streamArgumentType(std::ostream& os, const unsigned pos, Args const& ...args) const
  {
    auto proc = [&os](auto& e) {
      using Log::Backend::toType;
      os << toType(e);
    };
    processArgument(proc,  pos, args... );
  }
  template<typename ...Args>
  void streamArgumentValue(std::ostream& os, const unsigned pos, Args const& ...args) const
  {
    auto proc = [&os](auto& e) {
      using Log::Backend::toValue;
      toValue(e).visit( detail::StreamVisitor{&os} );
    };
    processArgument( proc,  pos, args... );
  }


  template<typename ...Args>
  void formatBlock(std::ostringstream& os, detail::Segment const& segment, Args const& ...args) const
  {
    switch(segment.type_)
    {
      case detail::Segment::Type::String:
        os.write( segment.begin_, segment.end_ - segment.begin_ );
        return;
      case detail::Segment::Type::Value:
        streamArgumentValue(os, segment.referencedArgument_, args...);
        return;
      case detail::Segment::Type::TypeName:
        streamArgumentType(os, segment.referencedArgument_, args...);
        return;
    }
    BUT_ASSERT(!"missing type handle");
  }

  const detail::ParsedFormat<MaxSegments> ps_;
  char const* format_;
};


template<unsigned N, unsigned M>
std::string toValue(Parsed<N,M> const&) = delete;

template<unsigned N, unsigned M>
std::string toType(Parsed<N,M> const&) = delete;

}
}
