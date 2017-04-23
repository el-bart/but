#pragma once
#include "Parsed.hpp"
#include "detail/argumentsUsageCount.hpp"

namespace But
{
namespace Format
{

/** @brief MPL&PP-based builder of But::Format::Parsed, that allows compile-time format validation, against given set of arguments.
 *  @note if format syntax will be invalid, program compilation will be stopped with an error.
 */
#define BUT_FORMAT(fmt) ::But::Format::Parsed<::But::Format::detail::argumentsUsageCount<sizeof(fmt)/2+1>(fmt)>{fmt}

}
}
