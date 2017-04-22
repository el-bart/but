#pragma once
#include <string>
#include "detail/format.hpp"

namespace But
{

/** @brief formats given text with a set of parameters into a single string.
 *  @note format of the parameter is defined by its type, via toString() function. it
 *        is not specified for a given type usage. this way formatting for a given parameters
 *        is always constant.
 *  @note first argument is format, then arguments follow.
 *  @param fmt format is a string with positional arguments, in a form:
 *          $N - expands to N-th argument.
 *          ${N} - the same as $N.
 *          ${N#some text} - the same as $N, but allowing some textual description along (useful for translations!).
 *          ${TN} - prints type of N-th argument, using typeString() function and ADL.
 *          ${TN#some text} - the same as ${TN}, but allowing some textual description along (useful for translations!).
 *          ${VN} - the same as $N.
 *          ${VN#some text} - the same as $N, but allowing some textual description along (useful for translations!).
 *          $$ - liternal '$' character.
 */
template<typename ...Args>
std::string format(std::string const& fmt, Args&&... args)
{
  return detail::format< sizeof...(Args) >( fmt, 42, std::forward<Args>(args)... );
}


/** @brief MPL&PP-based version of But::format(), that allows compile-time format validation, against given set of arguments.
 *  @note if format syntax will be invalid or will not match provided arguments count (i.e. all arguments must be used at least once)
 *        program compilation will be stopped with an error.
 */
#define BUT_FORMAT(fmt) Format<::But::detail::countFormatArguments(fmt)>{fmt}

}
