#pragma once
#include <cassert>

namespace But
{

/** @brief assert() wrapper, that works well with both reagular and constexpr functions.
 */

#ifdef NDEBUG
// TODO: is it possible to enable constexpr assert only in constexpr context, in non-debug mode?
#define BUT_ASSERT(cond) do { } while(false)
#else
#define BUT_ASSERT(cond) do { if(not (cond)) assert(!#cond); } while(false)
#endif

}
