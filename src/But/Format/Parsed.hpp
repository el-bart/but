#pragma once
#include <string>
#include <sstream>
#include "But/assert.hpp"
#include "But/Exception.hpp"
#include "detail/parse.hpp"
#include "Invalid.hpp"
#include "detail/argumentsCount.hpp"

namespace But
{
namespace Format
{

/** @brief represents a parsed format, from a given text with. set of parameters can be applied to get a final message.
 *  @note format of the parameter is defined by its type, via toFieldInfo() function. it
 *        is not specified for a given type usage. this way formatting for a given parameters
 *        is always constant.
 */
template<size_t ArgumentsCount, size_t MaxSegments>
class ParsedCompiletime final
{
public:
  using const_iterator = typename Container::ArrayWithSize<detail::Segment, MaxSegments>::const_iterator;

  /** @brief parses input format and constructs the object.
   *  @param format - format is a string with positional arguments, in a form:
   *          $N - expands to N-th argument.
   *          ${N} - the same as $N.
   *          ${N#some text} - the same as $N, but allowing some textual description along (useful for translations!).
   *          $$ - liternal '$' character.
   *  @note all numbers are 0-based (i.e. 1st argument has index 0).
   *  @note ADL-based to_string(x) free function is used to convert 'X' into a string. strings are then used to add to a stream.
   */
  constexpr explicit ParsedCompiletime(char const* format):
    ps_{ detail::parseCt<MaxSegments>(format) },
    format_{format}
  {
    BUT_ASSERT( expectedArguments() == detail::argumentsCount(ps_) );
  }

  auto inputFormat() const { return format_; }
  static constexpr size_t expectedArguments() { return ArgumentsCount; }

  const_iterator begin() const { return ps_.segments_.begin(); }
  const_iterator end() const { return ps_.segments_.end(); }

private:
  const detail::ParsedFormatCt<MaxSegments> ps_;
  char const* format_;
};



/** @brief ParsedCompiletime-equivalent, but working with runtime arguments.
 */
class ParsedRuntime final
{
public:
  using const_iterator = std::vector<detail::Segment>::const_iterator;

  BUT_DEFINE_EXCEPTION(ArityError, Exception, "invalid number of arguments for a format");

  explicit ParsedRuntime(std::string format):
    ps_{ detail::parseRt( format.c_str() ) },
    format_{ std::move(format) }
  {
    BUT_ASSERT( expectedArguments() == detail::argumentsCount(ps_) );
  }

  auto inputFormat() const { return format_; }
  size_t expectedArguments() const { return ps_.size(); }
  void validateArgumentsCount(const size_t arguments) const
  {
    if( arguments != expectedArguments() )
      BUT_THROW(ArityError, "expected " << expectedArguments() << " arguments - got " << arguments << " instead");
  }

  const_iterator begin() const { return ps_.segments_.begin(); }
  const_iterator end() const { return ps_.segments_.end(); }

private:
  const detail::ParsedFormatRt ps_;
  std::string format_;
};

}
}
