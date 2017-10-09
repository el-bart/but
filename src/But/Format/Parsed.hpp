#pragma once
#include <string>
#include <sstream>
#include "But/assert.hpp"
#include "detail/parse.hpp"
#include "detail/argumentsCount.hpp"
#include "detail/allArgumentsUsed.hpp"
#include "Invalid.hpp"
#include "detail/StreamVisitor.hpp"

namespace But
{
namespace Format
{

/** @brief represents a parsed format, from a given text with. set of parameters can be applied to get a final message.
 *  @note format of the parameter is defined by its type, via toFieldInfo() function. it
 *        is not specified for a given type usage. this way formatting for a given parameters
 *        is always constant.
 */
class Parsed final
{
public:
  /** @brief parses input format and constructs the object.
   *  @param format - format is a string with positional arguments, in a form:
   *          $N - expands to N-th argument.
   *          ${N} - the same as $N.
   *          ${N#some text} - the same as $N, but allowing some textual description along (useful for translations!).
   *          $$ - liternal '$' character.
   *  @note all numbers are 0-based (i.e. 1st argument has index 0).
   *  @note ADL-based to_string(x) free function is used to convert 'X' into a string. strings are then used to add to a stream.
   */
  constexpr explicit Parsed(char const* format):
    ps_{ detail::parse<MaxSegments>(format) },
    format_{format}
  { }

  auto inputFormat() const { return format_; }
  static constexpr auto expectedArguments() { return ArgumentsCount; }

  template<typename ...Args>
  std::string format(Args const& ...args) const
  {
    static_assert( sizeof...(args) == expectedArguments(), "arity missmatch between provided format and arguments to be formated" );
    BUT_ASSERT( expectedArguments() == detail::argumentsCount(ps_) );
    using std::to_string;
    const std::vector<std::string> arguments{ to_string(args)... };
    return formatImpl(arguments);
  }

private:
  std::string formatImpl(std::vector<std::string> const& args) const
  {
    BUT_ASSERT( expectedArguments() == args.size() );
    std::ostringstream os;
    for(auto& e: ps_.segments_)
      formatBlock(os, e, arguments);
    return os.str();
  }

  void formatBlock(std::ostringstream& os, detail::Segment const& segment, std::vector<std::string> const& arguments) const
  {
    switch(segment.type_)
    {
      case detail::Segment::Type::String:
        os.write( segment.begin_, segment.end_ - segment.begin_ );
        return;
      case detail::Segment::Type::Value:
        os << arguments[segment.referencedArgument_];
        return;
    }
    BUT_ASSERT(!"missing type handle");
  }

  const detail::ParsedFormat<MaxSegments> ps_;
  char const* format_;
};

}
}
