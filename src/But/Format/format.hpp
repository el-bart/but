#pragma once
#include "detail/formatImpl.hpp"

namespace But
{
namespace Format
{

/** @brief MPL&PP-based builder of But::Format::Parsed, that allows compile-time format validation, against given set of arguments.
 *  @note if format syntax will be invalid, program compilation will be stopped with an error.
 */
#define BUT_FORMAT(fmt) \
            BUT_FORMAT_DETAIL_FORMAT_IMPL( fmt, ((sizeof(fmt)/3+1)*2) )

}
}
